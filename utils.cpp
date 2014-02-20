#include "utils.h"

//using namespace arma;
#if 0
/* Open stereo file and place channels in rows of a matrix */
sf_count_t load_stereo_file_into_matrix(SNDFILE *sndfile, sf_count_t nframes, mat& data )
{
  int count = 0;
  /* A frame contains samples for all channels at time t */
  double frame[2];
  /* de-interleave data and place into 2 row matrix */
  for(count = 0; count < nframes; count++)
  {
      /* read interleaved frames one-by-one and de-interleave */
      sf_readf_double( sndfile, frame, (sf_count_t)1);
      data(0,count) = frame[0];
      data(1,count) = frame[1];
  }
  return count;
}
#endif
void writebits(unsigned int bits[], unsigned int nbits, const char* fn)
{
	FILE * f = fopen(fn, "wb");
	if (!f) {
		printf("Error: Can't open %s for writing.\n", fn);
		exit(1);
	}

	unsigned int t = 32000; // special case to represent packed bits

	fwrite(&t, 4, 1, f);
	fwrite(&nbits, 4, 1, f);

	for (unsigned int i = 0; i < nbits; i++) {
		fwrite(&(bits[i]), 4, 1, f);
	}

	fflush(f);
	fclose(f);
}

#if 0
// Create a hamming window of windowLength samples in buffer
vec hamming(int window_sz) {
  vec r(window_sz);
  for(int i = 0; i < window_sz; i++) {
    r[i] = 0.54 - (0.46 * cos( 2 * datum::pi * (i / ((window_sz - 1) * 1.0))));
}
return r;
}

// Implementation from http://ofdsp.blogspot.co.il/2011/08/short-time-fourier-transform-with-fftw3.html
//void STFT(std::vector <float> *signal, int signalLength, int windowSize, int hopSize) {

void spectrogram(const mat& S, int window_sz, int hop_sz, mat& result)
{
    // Create a hamming window of appropriate length
    vec window = hamming(window_sz);
    int chunkPosition = 0;
    int readIndex = 0;
    // Should we stop reading in chunks? 
    int bStop = 0;
    int numChunks = 0;

    // Process each chunk of the signal
    int len = S.n_elem;
    //std::cout << "spectrogram(): S.n_elem: " << len << std::endl;
    while(chunkPosition < len && !bStop) {
       // Copy the chunk into our buffer
    vec D(window_sz);
    for(int i = 0; i < window_sz; i++) {

        readIndex = chunkPosition + i;

        if(readIndex < len) {
            D[i] = S[readIndex] * window[i]; 
        } else {
            D[i] = 0.0; 
            bStop = 1;
        }
    }
    cx_mat Y =  fft(D);
  
  // Copy the first (windowSize/2 + 1) data points into your spectrogram.
  // We do this because the FFT output is mirrored about the nyquist 
  // frequency, so the second half of the data is redundant. This is how
  // Matlab's spectrogram routine works.
    vec r = real(Y);
    vec rr = r.subvec(0, window_sz/2 + 1);
    //std::cout << "r : " << r.t() << std::endl << "rr: " << rr.t() << std::endl;
    //std::cout << " n_cols: " << rr.n_cols << " n_rows: " << rr.n_rows << std::endl;
    //std::cout << "chunkPosition: " << chunkPosition << " numChunks: " << numChunks << std::endl; 
    result.insert_cols( numChunks, rr );
    chunkPosition += hop_sz;
    numChunks++;

  } // Excuse the formatting, the while ends here.

}

#endif
