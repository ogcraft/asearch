#ifndef __utils_h__
#define __utils_h__

////////////////////////////
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

#include <sndfile.h>
//#include <armadillo>

#define ARRAY_LEN(x)  ((int) (sizeof (x) / sizeof (x [0])))
#define MAX(x,y)    ((x) > (y) ? (x) : (y))
#define MIN(x,y)    ((x) < (y) ? (x) : (y))

#ifdef _WIN32
//float round(float x)
//{
//   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
//}
static inline double round(double val)
{    
    return floor(val + 0.5);
}
#endif

/* Open stereo file and place channels in rows of a matrix */
//void spectrogram(const arma::mat& S, int window_sz, int hop_sz, arma::mat& result);
void writebits(unsigned int bits[], unsigned int nbits, const char * fn);


#endif // __utils_h__
