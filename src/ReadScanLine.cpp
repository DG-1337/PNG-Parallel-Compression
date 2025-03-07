#include "../include/lodepng.h"  // Use double quotes for local files
#include <iostream>
#include <vector>

using namespace std; 

struct Pixel {
    unsigned r, g, b, a;
};

// creates a blue gradient png to use for testing
void createTestPNG(const string fileName, unsigned w, unsigned h) {
    vector <unsigned char> image(w * h * 4); 

    for (unsigned y = 0; y < h; ++y) {
        for (unsigned x = 0; x < w; ++x) {
            unsigned index = (y * w + x) * 4;

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


int main(void) {
    const char* newFileName = "test_image.png";

    createTestPNG(newFileName, 4, 4); 
    vector<vector<Pixel> > pixels = readScanLines(newFileName); 

    // used to show the vector values after reading each scanline
    // for (size_t y = 0; y < pixels.size(); y++) {
    //     for (size_t x = 0; x < pixels[y].size(); x++) {
    //         Pixel p = pixels[y][x];
    //         cout << "(" << (int)p.r << "," << (int)p.g << "," << (int)p.b << "," << (int)p.a << ") ";
    //     }
    //     cout << endl; 
    // }
    
    return 0; 
}
