#include "../include/lodepng.h"  // Use double quotes for local files
#include <iostream>
#include <vector>

using namespace std; 

struct Pixel {
    unsigned char r, g, b, a;
};

void createTestPNG(const string fileName, unsigned w, unsigned h) {
    vector <unsigned char> image(w * h * 4); 

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            size_t index = (y * w + x) * 4;

            image[index] = x * w; 
            image[index + 1] = y * 16; 
            image[index + 2] = 128; 
            image[index + 3] = 255; 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h); 
    if(err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } else {
        cout << "Image saved as " << fileName << endl;
    }
}

void readScanLines(const string fileName) {
    vector <unsigned char> image;
    unsigned w, h; 
    
    unsigned err = lodepng::decode(image, w, h, fileName);
    if(err) {
        cout << "Decoding error " << err << ": " << lodepng_error_text(err) << endl;
        return; 
    }    


    // Process each scanline
    for (unsigned y = 0; y < h; y++) {
        cout << "Scanline " << y << ": ";
        unsigned start_index = y * w * 4;  // Starting index for the row

        for (unsigned x = 0; x < w; x++) {
            unsigned index = start_index + x * 4; // Compute pixel index
            unsigned char r = image[index];
            unsigned char g = image[index + 1];
            unsigned char b = image[index + 2];
            unsigned char a = image[index + 3];

            cout << "(" << (int)r << "," << (int)g << "," << (int)b << "," << (int)a << ") ";
        }
        cout << endl;
    }
}

int main(void) {
    const char* newFileName = "test_image.png";

    createTestPNG(newFileName, 4, 4); 
    readScanLines(newFileName); 

    return 0; 
}