#include "../include/lodepng.h"  // Use double quotes for local files
#include <iostream>
#include <vector>

using namespace std; 

struct Pixel {
    unsigned r, g, b, a;
};

// reads a png line by line and returns its rbga pixel values in a 2d vector representing the image 
vector<vector<Pixel> > readScanLines(const string fileName) {
    vector<unsigned char> image;
    unsigned w, h;

    unsigned err = lodepng::decode(image, w, h, fileName);
    if (err) {
        cout << "Decoding error " << err << ": " << lodepng_error_text(err) << endl;
        return vector<vector<Pixel> >();
    }

    vector<vector<Pixel> > pixels(h, vector<Pixel>(w)); // Pre-allocate

    // Iterates over each row of the image
    for (unsigned y = 0; y < h; y++) {
        cout << "Scanline " << y << ": ";  
        unsigned start_index = y * w * 4;

        // iterates over the each pixel for a row 
        for (unsigned x = 0; x < w; x++) {
            unsigned index = start_index + x * 4;
            Pixel p = {image[index], image[index + 1], image[index + 2], image[index + 3]};
            pixels[y][x] = p; 
            cout << "(" << (int)p.r << "," << (int)p.g << "," << (int)p.b << "," << (int)p.a << ") "; 
        }
        cout << endl;
    }
    
    return pixels;
}


