#include "../libs/lodepng.h" 
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

#ifndef READ_SCAN_LINE_H;
#define READ_SCAN_LINE_H;

using namespace std; 

struct Pixel {
    unsigned r, g, b, a;
};

struct ImageData {
    vector<unsigned char> image; 
    unsigned w, h; 
    const string fileName;
};

ImageData readScanLines(const string fileName);
vector<unsigned char> getSingleScanline(vector<unsigned char> image, unsigned w, unsigned currRow);
vector<vector<Pixel> > mt_readScanLines(const string fileName);
vector<vector<Pixel> > st_readScanLines(const string fileName);

#endif;