#include <iostream>
#include "../include/ReadScanLine.h"

using namespace std; 

struct Pixel pixel; 

void printPixels(vector<vector<Pixel> > pixels) {
    for (size_t y = 0; y < pixels.size(); y++) {
        for (size_t x = 0; x < pixels[y].size(); x++) {
            Pixel p = pixels[y][x];
            cout << "(" << (int)p.r << "," << (int)p.g << "," << (int)p.b << "," << (int)p.a << ") ";
        }
        cout << endl; 
    }
}

int main() {
    const string filename = "test_image.png"; 
    vector<vector<Pixel> > st_pixels = st_readScanLines(filename);      // Single-Threaded
    vector<vector<Pixel> > mt_pixels = mt_readScanLines(filename);      // Multi-Threaded 

    cout << "Single-Threaded: " << endl; 
    printPixels(st_pixels);

    cout << endl; 

    cout << "Multi-Threded: " << endl; 
    printPixels(mt_pixels);

    return 0; 
}
