#include <iostream>
#include <fstream>
#include "../include/read_scan_line.h"
#include "../include/compression.h"
#include "../include/adaptive_filter.h"

using namespace std; 

void write_chunk(ofstream &file, const string &type, const vector<unsigned char> &data) {
    uint32_t length = htonl(data.size());
    file.write(reinterpret_cast<char *>(&length), sizeof(length));
    file.write(type.c_str(), 4);
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    
    uint32_t crc = crc32(0, reinterpret_cast<const Bytef *>(type.c_str()), 4);
    crc = crc32(crc, reinterpret_cast<const Bytef *>(data.data()), data.size());
    crc = htonl(crc);
    file.write(reinterpret_cast<char *>(&crc), sizeof(crc));
}

void write_png(const string &filename, uint32_t width, uint32_t height, const vector<unsigned char> &image_data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    // PNG Signature
    const unsigned char png_signature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    file.write(reinterpret_cast<const char *>(png_signature), 8);

    // IHDR Chunk
    vector<unsigned char> ihdr(13);
    *reinterpret_cast<uint32_t *>(&ihdr[0]) = htonl(width);
    *reinterpret_cast<uint32_t *>(&ihdr[4]) = htonl(height);
    ihdr[8] = 8;   // Bit depth
    ihdr[9] = 6;   // Color type (RGBA)
    ihdr[10] = 0;  // Compression method
    ihdr[11] = 0;  // Filter method
    ihdr[12] = 0;  // Interlace method
    write_chunk(file, "IHDR", ihdr);

    // IDAT Chunk (Uncompressed Data)
    vector<unsigned char> idat;
    idat.push_back(0x78); // zlib header (Deflate, no compression)
    idat.push_back(0x01);
    idat.insert(idat.end(), image_data.begin(), image_data.end());
    write_chunk(file, "IDAT", idat);

    // IEND Chunk
    write_chunk(file, "IEND", {});
    
    file.close();
    cout << "PNG written successfully: " << filename << endl;
}

int main(int argc, char* argv[]) {
    const string filename = argv[1]; 
    ImageData original_image = readScanLines(filename);

    cout << original_image.w  << " "  << original_image.h <<  " " << original_image.image.size() << endl;
    unsigned int num_cores = thread::hardware_concurrency();

    //vector<vector<Pixel> > st_pixels = st_readScanLines(filename);      Single-Threaded, Outdated
    //vector<vector<Pixel> > mt_pixels = mt_readScanLines(filename);      Multi-Threaded, Outdated

    ImageData filtered_image = adaptiveFilter(original_image);
    cout << filtered_image.w << " " << filtered_image.h << " " << filtered_image.image.size() << endl;

    vector<unsigned char> compressed_image;
    compressed_image = compress_image(filtered_image.image);
    cout << compressed_image.size() << endl;
    
    write_png(("compressed.png"), original_image.w, original_image.h, compressed_image);

    return 0; 
}
