recordkfn = "../../../asearchdata/makarevich-litsa-recorded-fingerpoints.bin"
fullsamplekfn = "../../../asearchdata/makarevich-litsa-recorded-fingerpoints.bin"

recordkfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-fingerpoints.bin"
fullsamplekfn = "/Users/olegg/asearchdata/mercury_rising/VTS_01_4-rec-5513-fingerpoints.bin"


keys_in_sec = 86
sec_per_sample =  0.01164 

function read_keys_from_file(filename::String)
    io = open(filename)
    magic = read(io, Int32)
    n = read(io, Int32)
    a = Array(Uint32, n)
    i = 0        
    while !eof(io)
        i += 1
        a[i] = read(io, Uint32)
    end
    return a
end

function calc_dist(start_pos, record_keys, sample_keys, nsec, shift_sec)
	w = nsec * keys_in_sec
    shift = shift_sec * keys_in_sec
    acc = 0
	for i = 1:w
		d =  count_ones(record_keys[start_pos+i] $ sample_keys[i+shift]) # xor
		acc = acc + d
	end	
	return int(acc/w)
end

function match_old(record_fn, sample_fn, nsec)
    record_keys = read_keys_from_file(record_fn)
    sample_keys = read_keys_from_file(sample_fn)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    @printf "Read keys: %d from: %s\n"  nrecords  record_fn
    @printf "Read keys: %d from: %s\n" nsamples sample_fn
    nrecords = 10000
    diffs = fill(33, nrecords)
    diffs1 = fill(33, nrecords)
    for i = 1:(nrecords-(nsec+1) * keys_in_sec)
	    diffs[i] = calc_dist(i, record_keys, sample_keys, nsec, 0)
	    diffs1[i] = calc_dist(i, record_keys, sample_keys, nsec, 5)
    end
    @printf "Collected %d diffs\n"  length(diffs)
    m,index=findmin(diffs)
    m1,index1=findmin(diffs1)
    @printf "Found min: %d index: %d sec: %f\n\n" m index index*sec_per_sample
    @printf "Found min: %d index: %d sec: %f\n\n" m1 index1 index1*sec_per_sample
    @printf "Diff t1-t2: %f\n\n" (index1-index)*sec_per_sample
    return diffs, diffs1
end

function match_single_pass(record_keys, sample_keys, nsec, sec)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    diffs = fill(33, nrecords)
    max_i = (nrecords-(nsec+1) * keys_in_sec)
    @printf "nrecords: %d nsamples:%d max_i: %d\n" nrecords nsamples max_i
    k = 1
    for i = 1:max_i
	    diffs[i] = calc_dist(i, record_keys, sample_keys, nsec, 0)
        k += 1
    end
    @printf "Collected %d diffs k: %d\n"  length(diffs) k
    m,index=findmin(diffs)
    @printf "Found sec: %f min: %d index: %d \n" index*sec_per_sample m index
    diff_in_secs = (sec - index * sec_per_sample)
    @printf "Diff secs_diff: %f "  diff_in_secs
    if abs(diff_in_secs) > 1
        @printf " ====== Not Found\n"
    else
        @printf "\n"
    end
    return diffs
end
function match1(record_keys, sample_keys, nsec, sec)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    diffs = fill(33, nrecords)
    diffs1 = fill(33, nrecords)
    for i = 1:(nrecords-(nsec+1) * keys_in_sec)
	    diffs[i] = calc_dist(i, record_keys, sample_keys, nsec, 0)
	    diffs1[i] = calc_dist(i, record_keys, sample_keys, nsec, 5)
    end
    @printf "Collected %d diffs\n"  length(diffs)
    m,index=findmin(diffs)
    m1,index1=findmin(diffs1)
    @printf "Found sec: %f min: %d index: %d \n" index*sec_per_sample m index
    @printf "Found sec: %f min: %d index: %d \n" index1*sec_per_sample m1 index1
    diff_in_sec = (index1-index)*sec_per_sample
    @printf "Diff t1-t2: %f secs_diff: %f " diff_in_sec (sec - index * sec_per_sample)
    if abs(diff_in_sec) > 6
        @printf " ====== Not Found\n"
    else
        @printf "\n"
    end
    return diffs, diffs1
end

function match(record_fn, sample_fn, nsec)
    record_keys = read_keys_from_file(record_fn)
    sample_keys = read_keys_from_file(sample_fn)
    nrecords = length(record_keys)
    nsamples = length(sample_keys)
    @printf "Read keys: %d from: %s secs: %f\n"  nrecords  record_fn nrecords * sec_per_sample
    @printf "Read keys: %d from: %s secs: %f\n" nsamples sample_fn nsamples * sec_per_sample
    sample_size = 20 * keys_in_sec
    @printf "Sample size: %d secs: %f\n" sample_size sample_size * sec_per_sample 
    start_sample = 100
    from_end_of_record = nrecords
    i = 1
    while from_end_of_record > sample_size 
        @printf "\n---------------- Test %d -----------------\n" i
        #@printf "start_sample: %d from end: %d\n" start_sample from_end_of_record
        @printf "Looking for sec: %f (%d) from total: %f\n" start_sample * sec_per_sample start_sample nsamples * sec_per_sample

        diff =  match_single_pass(record_keys, sample_keys[start_sample:end], nsec, start_sample * sec_per_sample)

        start_sample += sample_size
        from_end_of_record = (nsamples - 10) - (start_sample + sample_size)
        i += 1
    end
end

function test1()
    match(recordkfn, fullsamplekfn, 3)
end

