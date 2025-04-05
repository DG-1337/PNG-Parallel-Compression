#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "../include/read_scan_line.h"
#include "../include/compression.h"
#include "../include/filter.h"
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

    // compress image scanlines
    vector <unsigned char> compressed_data = compress_image(image_data); 

    // IDAT chunk 
    write_chunk(file, "IDAT", compressed_data);

    // IEND Chunk
    write_chunk(file, "IEND", {});
    
    file.close();
    cout << "PNG written successfully: " << filename << endl;
}

vector <unsigned char> single_filtered_method(const vector<unsigned char> image, unsigned w, unsigned h, int filterMethod) {
    vector <unsigned char> result; 

    if(image.size() != w * h * 4) {
        throw runtime_error("Error: image size does not match with expected size (w * h * 4)");
    }

    for (unsigned y = 0; y < h; ++y) {
        vector<unsigned char> row(image.begin() + y * w * 4, image.begin() + (y + 1) * w * 4); 
        vector<unsigned char> prevRow;

        if (y > 0) {
            prevRow = vector<unsigned char>(image.begin() + (y - 1) * w * 4, image.begin() + y * w * 4);
        }

        vector<unsigned char> filteredRow;

        if (filterMethod == 0) {
            filteredRow = applyNoneFilter(row);
        } else if (filterMethod == 1) {
            filteredRow = applySubFilter(row, w);
        } else if (filterMethod == 2) {
            filteredRow = applyUpFilter(row, prevRow);
        } else if (filterMethod == 3) {
            filteredRow = applyAverageFilter(row, prevRow, w);
        } else if (filterMethod == 4) {
            filteredRow = applyPaethFilter(row, prevRow, w);
        } else {
            throw runtime_error("Unknown filter method");
        }

        result.insert(result.end(), filteredRow.begin(), filteredRow.end());  // append result with the filter row 
    }

    // sanity check 
    size_t expected_size = h * (1 + w * 4); 
    if(result.size() != expected_size) {
        throw runtime_error("Error: Filtered scanline != Original image"); 
    }

    return result; 
}

// helper function to compare sizes between uncompressed and compressed image files 
uintmax_t get_file_size(const std::string &path) {
    struct stat stat_buf;
    int rc = stat(path.c_str(), &stat_buf);
    return rc == 0 ? static_cast<uintmax_t>(stat_buf.st_size) : 0;
}


int main(int argc, char* argv[]) {
    // user doesn't type in file name exit out of program
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input.png> " << endl; 
        return 1; 
    }    

    string const filename = argv[1]; 
    ImageData original_image = readScanLines(filename);

    cout << original_image.w  << " "  << original_image.h <<  " " << original_image.image.size() << endl;
    unsigned int num_cores = thread::hardware_concurrency();

    // applys only the sub filter to every scan-line 
    vector<unsigned char> sub_filtered_img = single_filtered_method(original_image.image, original_image.w, original_image.h, 1);  

    write_png(("compressed.png"), original_image.w, original_image.h, sub_filtered_img);

    // print file size of two images
    uintmax_t original_size = get_file_size(filename);
    uintmax_t compressed_size = get_file_size("compressed.png");
    
    // prints difference between uncompressed original image and filtered compressed image 
    cout << "Original size:   " << original_size << " bytes" << endl;
    cout << "Compressed size: " << compressed_size << " bytes" << endl;
    cout << "Compression ratio: " << (100.0 * compressed_size / original_size) << "% of original size" << endl;
    
    return 0; 
}
