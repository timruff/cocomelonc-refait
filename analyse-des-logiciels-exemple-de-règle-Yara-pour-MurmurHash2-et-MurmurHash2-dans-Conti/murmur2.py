def murmurhash2(data,seed):
    length = len(data)
    #seed = 0x9747b28c
    m = 0x5bd1e995
    r = 24
    h = seed ^ length
    length4 = length // 4
    
    for i in range(length4):
        i4 = i * 4
        k = ((data[i4 + 0] & 0xff) +
            ((data[i4 + 1] & 0xff) << 8) +
            ((data[i4 + 2] & 0xff) << 16) +
            ((data[i4 + 3] & 0xff) << 24))
        k &= 0xffffffff
        k *= m
        k &= 0xffffffff
        k ^= (k % 0x100000000) >> r # k ^= k >>> r
        k &= 0xffffffff
        k *= m
        k &= 0xffffffff

        h *= m
        h &= 0xffffffff
        h ^= k
        h &= 0xffffffff

    # Handle the last few bytes of the input array
    extra_bytes = length % 4
    if extra_bytes >= 3:
        h ^= (data[(length & ~3) + 2] & 0xff) << 16
        h &= 0xffffffff
    if extra_bytes >= 2:
        h ^= (data[(length & ~3) + 1] & 0xff) << 8
        h &= 0xffffffff
    if extra_bytes >= 1:
        h ^= (data[length & ~3] & 0xff)
        h &= 0xffffffff
        h *= m
        h &= 0xffffffff

    h ^= (h % 0x100000000) >> 13 # h >>> 13;
    h &= 0xffffffff
    h *= m
    h &= 0xffffffff
    h ^= (h % 0x100000000) >> 15 # h >>> 15;
    h &= 0xffffffff

    return h

h = murmurhash2(b"MessageBoxA",0)
print ("%x" % h)
print ("%d" % h)
