#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;
#ifdef _WIN32 
unsigned int bit_count(unsigned int x)
{
    return bitset<32>(x).count();
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

const int PROBE_SZ = 15 * 86;

pair<int,int> min_diffs(const vector< pair<int, int> >& diffs)
{
	pair<int,int> m = make_pair(10000,0);
	for(vector< pair<int, int> >::const_iterator it = diffs.begin();
			it != diffs.end(); ++it) {
		if(it->first < m.first) {
			m.first = it->first;
			m.second = it->second;
		}
	}
	return m;
}

int calc_distance(int s, vector<uint32_t>& track_keys, vector<uint32_t>& sample_keys, int probe_sz)
{
	int acc = 0;
	for(int i = 0; i < probe_sz; i++) {
		int d =  bit_count(track_keys[s+i] ^ sample_keys[i]);
		acc = acc + d;
	}
	return acc / probe_sz;
}


int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Usage: amatch track_fn sample_fn" << endl;
        return 1;
    }
	string track_fn(argv[1]);
	string sample_fn(argv[2]);
	//cout << "Track: " << track_fn << endl;
	//cout << "Sample: " << sample_fn << endl;
    std::ifstream itrack(track_fn, std::ios::binary);
	vector<uint32_t> track_keys;
	vector<uint32_t> sample_keys;
    uint32_t n = 0;
	uint32_t magic = 0; 
	itrack.read((char*)&magic, sizeof(magic));
	uint32_t track_len = 0;
	itrack.read((char*)&track_len, sizeof(track_len));

    while(itrack.read((char *)&n, sizeof(n))) {	
        track_keys.push_back(n);
	}

	if(track_keys.size() != track_len) {
		cout << "Wrong number of keys read: " << track_keys.size() << " != " << track_len << endl; 
	}

	cout << "From " << track_fn << " read " << track_keys.size() << endl;

    std::ifstream isample(sample_fn, std::ios::binary);
	
	isample.read((char*)&magic, sizeof(magic));
	uint32_t sample_len = 0;
	isample.read((char*)&sample_len, sizeof(sample_len));

    while(isample.read((char *)&n, sizeof(n))) {	
        sample_keys.push_back(n);
	}

	if(sample_keys.size() != sample_len) {
		cout << "Wrong number of keys read: " << sample_keys.size() << " != " << sample_len << endl; 
	}

	cout << "From " << sample_fn << " read " << sample_keys.size() << endl;
#if 0
	for(size_t i=0; i < 20; i++) {
		cout << i << ": " << track_keys[i] << endl;	
	}
#endif	
	
	vector< pair<int, int> > diffs;
	pair<int,int> m;
	int idx = 0;
	size_t i = 0;
	for(; i < track_keys.size() - 85*20 ; i++) {
		int d = calc_distance(i, track_keys, sample_keys, PROBE_SZ);
		diffs.push_back(make_pair(d,i));
	}
	cout << "Calculated " << diffs.size() << " diffs" << endl;
//end:
	m = min_diffs(diffs);
	cout << "Found min(diffs):" <<  m.first << " index: " <<  m.second << " sec: " << m.second * 0.01164 << endl; 
	
	return 0;
}
