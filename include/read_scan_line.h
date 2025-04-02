#include "../libs/lodepng.h" 
#include <iostream>
#include <vector>

#ifndef READ_SCAN_LINE_H;
#define READ_SCAN_LINE_H;

using namespace std; 

struct ImageData {
    vector<unsigned char> image; 
    unsigned w, h; 
    const string fileName;
};

ImageData readScanLines(const string fileName);
vector<unsigned char> getSingleScanline(vector<unsigned char> image, unsigned w, unsigned currRow);

#endif;