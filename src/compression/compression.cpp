#include "../../include/compression.h"

vector<unsigned char> compress_image(const vector<unsigned char> &input) {
    cout << "Original size: " << input.size() << endl;

    z_stream stream{};
    if (deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        throw runtime_error("deflateInit2 failed");
    }

    stream.next_in = const_cast<Bytef *>(input.data());
    stream.avail_in = input.size();

    vector<unsigned char> compressed_data(input.size() + (input.size() / 10) + 12);
    stream.next_out = compressed_data.data();
    stream.avail_out = compressed_data.size();

    if (deflate(&stream, Z_FINISH) != Z_STREAM_END) {
        deflateEnd(&stream);
        throw runtime_error("Compression failed");
    }

    compressed_data.resize(stream.total_out);
    deflateEnd(&stream);

    cout << "Compressed size: " << compressed_data.size() << endl;  

    return compressed_data;
}