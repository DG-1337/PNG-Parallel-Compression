#include "../../include/compression.h"
vector<unsigned char> compress_image(const vector<unsigned char> &input) {
    cout << "Original size: " << input.size() << endl;

    uLongf compressed_size = compressBound(input.size());
    vector<unsigned char> compressed(compressed_size); 

    z_stream stream{};
    if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw runtime_error("deflateInit2 failed");
    }

    memset(&stream, 0, sizeof(stream));
    deflateInit(&stream, Z_BEST_COMPRESSION);

    stream.next_in = const_cast<Bytef *>(input.data());
    stream.avail_in = input.size();
    stream.next_out = compressed.data(); 
    stream.avail_out = compressed_size; 

    int result = deflate(&stream, Z_FINISH);

    if(result != Z_STREAM_END) {
        deflateEnd(&stream);
        throw runtime_error("Zlib compression failed while trying to preform deflate()");
    }

    deflateEnd(&stream);
    compressed.resize(stream.total_out);
    return compressed;
}