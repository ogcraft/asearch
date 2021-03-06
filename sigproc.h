/* -*-  Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t -*- */
/*
Author: Yan Ke
May 2004
*/

#ifndef SIGPROC_H
#define SIGPROC_H

#include "filters.h"

void dump_sf_info(const SF_INFO& sf_info, const char* msg);
/** Read wave from file. */
float * wavread(const char * fn, unsigned int * nsamples, unsigned int * freq);


/** Given a set of filters to use and a wave sample, convert wave sample to
 * bit representation.
 */
unsigned int * wav2bits(vector<Filter> & filters, float * samples,
			unsigned int nsamples, unsigned int freq,
			unsigned int * nbits);


#endif
