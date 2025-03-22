#include <iostream>
#include <vector>
#include "../libs/lodepng.h"

using namespace std; 

// creates rainbow gradient test image 
void createTestPNG1(const string fileName, unsigned w, unsigned h) {
    vector<unsigned char> image(w * h * 4);

    for (unsigned y = 0; y < h; ++y) {
        for (unsigned x = 0; x < w; ++x) {
            unsigned index = (y * w + x) * 4;
            image[index + 0] = x * w;               // Red
            image[index + 1] = y * 16;              // Green
            image[index + 2] = 128;                 // Blue
            image[index + 3] = 255;                 // Alpha 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } 
}

// Creates a gray non-transparent image 
void createSolidGrayscalePNG(const string fileName, unsigned w, unsigned h) {
    vector <unsigned char> image(w * h * 4); 

    for (unsigned y = 0; y < h; y ++) {
        for (unsigned x = 0; x < w; x++) {
            unsigned index = 4 * (y * w + x); 
            unsigned char gray = (x + y) % 256; 
            image[index + 0] = gray; 
            image[index + 1] = gray; 
            image[index + 2] = gray;
            image[index + 3] = 255; 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } 
}



int main() {

    const string FOLDER_PATH = "tests/img/";

    const string TEST1 = FOLDER_PATH + "test1.png"; 
    const string TEST2 = FOLDER_PATH + "test2.png"; 

    unsigned w1 = 16; 
    unsigned h1 = 16; 

    unsigned w2 = 256;
    unsigned h2 = 256; 

    createTestPNG1(TEST1, h1, w1); 
    createSolidGrayscalePNG(TEST2, h2, w2); 

    return 0; 
}
