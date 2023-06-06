#deflate micropython code
import zlib

def deflate_compress(data):
    compressed_data = zlib.compress(data, level=9)
    return compressed_data

def deflate_decompress(compressed_data):
    decompressed_data = zlib.decompress(compressed_data)
    return decompressed_data

#The deflate_compress function takes in the data to be compressed and uses the zlib.compress
#function to compress it using the DEFLATE algorithm with the highest compression level (level 9).
#The resulting compressed data is returned.

#The deflate_decompress function takes in the compressed data and uses the zlib.decompress
#function to decompress it using the DEFLATE algorithm. The resulting decompressed data is returned.

#Note that the code assumes that the data to be compressed is a bytes object,
#and the output is a bytes object as well. If you need to use other types of objects,
#you may need to modify the code accordingly. */


#micropython code for compression and decompression using LZSS
import array

#max_window_size specifies the maximum size of the sliding window
#max_lookahead specifies the macimum number of bytes that can be considered for a match
#the input is a 'bytes' object and the output is a 'bytearray'

def lzss_compress(data, max_window_size=1024, max_lookahead=32):
    compressed_data = array.array('B')
    i = 0
    while i < len(data):
        window_start = max(0, i - max_window_size)
        window_end = i
        best_match = None
        for j in range(i+1, min(i+max_lookahead+1, len(data)+1)):
            lookahead = data[i:j]
            match_start = window_start
            while True:
                match_start = data.find(lookahead, match_start, window_end)
                if match_start == -1:
                    break
                match_length = len(lookahead)
                while match_start + match_length < i and match_length < max_lookahead:
                    if data[i+match_length] != data[match_start+match_length]:
                        break
                    match_length += 1
                if best_match is None or match_length > len(best_match[1]):
                    best_match = (i-match_start, lookahead[:match_length])
            if best_match is not None:
                break
        if best_match is not None:
            offset, match = best_match
            compressed_data.append(offset >> 8)
            compressed_data.append(offset & 0xFF)
            compressed_data.append(len(match) - 1)
            i += len(match)
        else:
            compressed_data.append(data[i])
            i += 1
    return compressed_data

#takes the compressed data and returns the decomressed data
def lzss_decompress(compressed_data, max_window_size=1024):
    decompressed_data = bytearray()
    i = 0
    while i < len(compressed_data):
        if compressed_data[i] == 0 and compressed_data[i+1] == 0:
            decompressed_data.append(compressed_data[i+2])
            i += 3
        else:
            offset = (compressed_data[i] << 8) | compressed_data[i+1]
            length = compressed_data[i+2] + 1
            i += 3
            for j in range(length):
                decompressed_data.append(decompressed_data[-offset])
    return decompressed_data
