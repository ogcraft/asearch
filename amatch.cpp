#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>

#ifdef _WIN32

#include <intrin.h>
const uint32_t  b1 = 0x55555555; // 0b01010101010101010101010101010101
const uint32_t  b2 = 0x33333333; //0b00110011001100110011001100110011
const uint32_t  b3 = 0x0f0f0f0f; //0b00001111000011110000111100001111
const uint32_t  b4 = 0x00ff00ff; //0b00000000111111110000000011111111
const uint32_t  b5 = 0x0000ffff; //0b00000000000000001111111111111111

unsigned int bit_count1(unsigned int x)
{
  x = (((x >> 1) & b1) + x & b1);
  x = (((x >> 2) & b2) + x & b2); 
  x = (((x >> 4) & b3) + x & b3); 
  x = (((x >> 8) & b4) + x & b4); 
  x = (((x >> 16)& b5) + x & b5); 
  return x;
}

unsigned int bit_count(unsigned int x)
{
    //return std::bitset<32>(x).count();
    return __popcnt(x);
}

static inline double round(double val)
{    
    return floor(val + 0.5);
}

#else
unsigned int bit_count(unsigned int x)
{
  x = (((x >> 1) & 0b01010101010101010101010101010101)
       + x       & 0b01010101010101010101010101010101);
  x = (((x >> 2) & 0b00110011001100110011001100110011)
       + x       & 0b00110011001100110011001100110011); 
  x = (((x >> 4) & 0b00001111000011110000111100001111)
       + x       & 0b00001111000011110000111100001111); 
  x = (((x >> 8) & 0b00000000111111110000000011111111)
       + x       & 0b00000000111111110000000011111111); 
  x = (((x >> 16)& 0b00000000000000001111111111111111)
       + x       & 0b00000000000000001111111111111111); 
  return x;
}
#endif

typedef std::vector<uint32_t> key_vector;
typedef std::vector< std::pair<int, int> > diff_vector;

const int keys_in_sec = 86;
const double sec_per_sample = (double) 0.01164; 
//const int PROBE_SZ = 15 * 86;

std::pair<int,int> min_diffs(const diff_vector& diffs)
{
	std::pair<int,int> m = std::make_pair(10000,0);
	for(diff_vector::const_iterator it = diffs.begin();
			it != diffs.end(); ++it) {
		if(it->first < m.first) {
			m.first = it->first;
			m.second = it->second;
		}
	}
	return m;
}

int calc_dist(size_t start_pos, const  key_vector& record_keys, const  key_vector& sample_keys,  size_t sample_key_start, int nsec, int shift_sec)
{
    size_t w = nsec * keys_in_sec;
    size_t shift = shift_sec * keys_in_sec;
    int acc = 0;
	for(size_t i = 0; i < w; i++) {
		int d =  bit_count(record_keys[start_pos+i] ^ sample_keys[sample_key_start + i + shift]);
		acc += d;
	}
	return round(acc/w);
}

size_t read_keys_from_file(const std::string& filename, key_vector& keys)
{
    std::ifstream ifile( filename, std::ios::binary);
	
    uint32_t magic = 0; 
	ifile.read((char*)&magic, sizeof(magic));
	
    uint32_t len = 0;
	ifile.read((char*)&len, sizeof(len));
    
    keys.reserve(len);
    uint32_t n = 0;
    while(ifile.read((char *)&n, sizeof(n))) {	
        keys.push_back(n);
	}

	if(keys.size() != len) {
		printf("Read wrong number of keys: %d != %d\n",keys.size(), len); 
	}

    return len; 
}

size_t match_single_pass(const key_vector& record_keys, const key_vector& sample_keys, size_t sample_key_start, int nsec, double sec)
{
    size_t nrecords = record_keys.size();
    size_t nsamples = sample_keys.size();

    diff_vector diffs(nrecords, std::make_pair(33,0));
    size_t max_i =  (nrecords-(nsec+1) * keys_in_sec);
    printf("nrecords: %d nsamples:%d max_i: %d\n", nrecords, nsamples, max_i);
    size_t i = 0;
    for(; i < max_i; i++ ) {
        int d = calc_dist(i, record_keys, sample_keys, sample_key_start, nsec, 0);
        diffs[i] = std::make_pair(d,i);
    }
    
    printf("Collected %d diffs i: %d\n", diffs.size(), i);
    
    std::pair<int,int> dp = min_diffs(diffs);
    
    int m = dp.first;
    int index = dp.second;
    
    printf("Found sec: %f at index: %d  minimum: %d\n", index*sec_per_sample, index, m);
    double diff_in_secs = (sec - index * sec_per_sample);
    printf("Diff secs_diff: %f \n",  diff_in_secs);
    
    std::string isfound = abs(diff_in_secs) > 1 ? "TEST FAILED !!!":"TEST PASSED";
    printf("%s\n", isfound.c_str());
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc < 3) {
        printf("Usage: amatch <track filename> <sample filename> [num of tests]");
        return 1;
    }
    int nsec = 10;
    int max_tryes = 2000;
    int start_sample = 100; // 5 * keys_in_sec;
    if(argc >= 4) {
        max_tryes = atoi(argv[3]);
    }
    if(argc >= 5) {
        nsec = atoi(argv[4]);
    }

    std::string track_fn(argv[1]);
	std::string sample_fn(argv[2]);
	key_vector track_keys;
	key_vector sample_keys;
    read_keys_from_file(track_fn, track_keys);
    read_keys_from_file(sample_fn, sample_keys);
    printf( "Read fpkeys: %d from: %s\n", track_keys.size(), track_fn.c_str());
    printf( "Read fpkeys: %d from: %s\n", sample_keys.size(), sample_fn.c_str());
	size_t nrecords = track_keys.size();
    size_t nsamples = sample_keys.size();
    size_t sample_size = 20 * keys_in_sec;
    printf("Sample size: %d secs: %f\n", sample_size, sample_size * sec_per_sample); 
    size_t from_end_of_record = nrecords;
    int i = 0;
    int ntest = 0;
    while( from_end_of_record > sample_size ) { 
        printf("\n---------------- Test %d -----------------\n", i);
        printf("Looking for sec: %f (%d) from total: %f\n", start_sample * sec_per_sample, start_sample, nsamples * sec_per_sample);
        
        //key_vector new_sample_keys(sample_keys.begin() + start_sample, sample_keys.end());

        match_single_pass(track_keys, sample_keys, start_sample, nsec, start_sample * sec_per_sample);

        start_sample += sample_size;
        from_end_of_record = (nsamples - 10) - (start_sample + sample_size);
        i++;
        ntest++;
        if(ntest >= max_tryes) {
            break;
        }
    }

	return 0;
}
