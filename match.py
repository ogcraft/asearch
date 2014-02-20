import os
import sys
import numpy as np

POPCOUNT_TABLE16 = [0] * 2**16

for index in xrange(len(POPCOUNT_TABLE16)):
    POPCOUNT_TABLE16[index] = (index & 1) + POPCOUNT_TABLE16[index >> 1]

def popcount32_table16(v):
    return (POPCOUNT_TABLE16[ v        & 0xffff] +
            POPCOUNT_TABLE16[(v >> 16) & 0xffff])

def calc_bites(x):
	return popcount32_table16(x)

#recordkfn = '../../../../asearchdata/prometheus-02-eng-dvd-fingerpoints.bin'
#samplekfn = '../../../../asearchdata/prometheus-ts-44-10-40sec-fingerpoints.bin'
#samplekfn = '../../../../asearchdata/prometheus-ts-44-150-200sec-fingerpoints.bin'

recordkfn = '../../../../asearchdata/makarevich-litsa-fingerpoints.bin'
#samplekfn = '../../../../asearchdata/makarevich-litsa-recorded-60-90sec-fingerpoints.bin'
samplekfn = '../../../../asearchdata/makarevich-litsa-recorded-128-134sec-fingerpoints.bin'
#samplekfn = '../../../../asearchdata/makarevich-litsa-recorded-9-14sec-fingerpoints.bin'
#samplekfn = '../../../../asearchdata/makarevich-litsa-recorded-101-120sec-fingerpoints.bin'


recf = open(recordkfn, "r")
a = np.fromfile(recf, dtype=np.uint32, count=2)
print "File %s contains %d fingerprints" % (recordkfn, a[1])
print a[0]
print a[1]

samplf = open(samplekfn, "r")
a = np.fromfile(samplf, dtype=np.uint32, count=2)
print "File %s contains %d fingerprints" % (samplekfn, a[1])
print a[0]
print a[1]

print "Loading data from ", recordkfn
record = np.fromfile(recf, dtype=np.uint32, count=-1)
print "Loaded %d items" % len(record)
print record[0]

print "Loading data from ", samplekfn
sample = np.fromfile(samplf, dtype=np.uint32, count=-1)
print "Loaded %d items" % len(sample)

def calc_dist1(s, record, sample):
	e = 2*86
	i = 0
	acc = 0
	for i in range(0,2*86):
		#d =  calc_bites(np.binary_repr(record[s+i] ^ sample[i],32))
		d =  calc_bites(record[s+i] ^ sample[i])
		acc = acc + d
		#print "d", d
	return acc/85

offset = 0
i = offset
diffs = []
for r in record[:-200]:
	d = calc_dist1(i, record, sample)
	diffs.append(d)
	i=i+1

print "Collected %d diffs" % len(diffs)

m = min(diffs)
idx = diffs.index(m);

print "min(diffs):", m, " index:", idx, " sec: ", idx * 0.01164 

#	if d < 5 :
#		print "@@@@@ Found sample: " , i
#		print "Record: ", np.binary_repr(r,32)
#		print "Sample: ", np.binary_repr(sample[0],32)
#

#offset = 2577 + 38
#for s in sample[:100]:
#	print np.binary_repr(record[offset + i],32), np.binary_repr(s,32)
#	i=i+1



print "-------------------------------"


