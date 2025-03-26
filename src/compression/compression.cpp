#include <iostream>
#include <thread>
#include <vector>
#include <zlib.h>

using namespace std;

void compress_chunk(const vector<unsigned char>& input, size_t start, size_t length, vector<unsigned char>& output) {
    z_stream stream{};
    stream.next_in = const_cast<Bytef*>(input.data() + start);
    stream.avail_in = length;

    uLong bound = deflateBound(&stream, length);
    output.resize(bound);

    stream.next_out = output.data();
    stream.avail_out = output.size();

    if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK)
        throw runtime_error("deflateInit failed");

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw runtime_error("deflate failed");
    }

    output.resize(stream.total_out);
    deflateEnd(&stream);
}