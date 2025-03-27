#include <iostream>
#include <vector>
#include "../libs/lodepng.h"

using namespace std; 

// creates rainbow gradient test image 
vector<unsigned char> createTestPNG1(const string fileName, unsigned w, unsigned h, unsigned alpha) {
    vector<unsigned char> image(w * h * 4);

    for (unsigned y = 0; y < h; ++y) {
        for (unsigned x = 0; x < w; ++x) {
            unsigned index = (y * w + x) * 4;
            image[index + 0] = x * w;               // Red channel
            image[index + 1] = y * 16;              // Green channel 
            image[index + 2] = 128;                 // Blue channel
            image[index + 3] = 255;                 // Alpha channel 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } 
    return image; 
}

// Creates a gray non-transparent image 
vector<unsigned char> createSolidGrayscalePNG(const string fileName, unsigned w, unsigned h, unsigned alpha) {
    vector <unsigned char> image(w * h * 4); 

    for (unsigned y = 0; y < h; y ++) {
        for (unsigned x = 0; x < w; x++) {
            unsigned index = 4 * (y * w + x); 
            unsigned char gray = (x + y) % 256; 
            image[index + 0] = gray; 
            image[index + 1] = gray; 
            image[index + 2] = gray;
            image[index + 3] = alpha; 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } 
    return image; 
}

// Creates checkered board pattern image 
vector<unsigned char> createCheckerBoardPNG(const string fileName, unsigned w, unsigned h, unsigned size, unsigned alpha) {
    vector <unsigned char> image(w * h * 4); 

    for (unsigned y = 0; y < h; y ++) {
        for (unsigned x = 0; x < w; x++) {
            unsigned index = 4 * (y * w + x);; 
            bool isWhite = ((x / size) % 2 == (y/size) % 2); 
            unsigned char color = isWhite ? 255: 0; 

            image[index + 0] = color; 
            image[index + 1] = color; 
            image[index + 2] = color; 
            image[index + 3] = alpha; 
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } 
    return image; 
}

int main() {

    const string FOLDER_PATH = "tests/img/";

    const string TEST1 = FOLDER_PATH + "test1.png"; 
    const string TEST2 = FOLDER_PATH + "test2.png";
    const string TEST3 = FOLDER_PATH + "test3.png";
    const string TEST4 = FOLDER_PATH + "test4.png";
    const string TEST5 = FOLDER_PATH + "test5.png";
    const string TEST6 = FOLDER_PATH + "test6.png";
    const string TEST7 = FOLDER_PATH + "test7.png";

    // Solid images 
    createTestPNG1(TEST1, 16, 16, 255);                     // creates solid image 16 x 16 
    createSolidGrayscalePNG(TEST2, 256, 256, 255);          // creates solid gray scaled image 256 x 256 
    createCheckerBoardPNG(TEST3, 128, 128, 16, 255);        // creates solid checkered image 128 x 128 

    // semi  transparent 
    createTestPNG1(TEST4, 16, 16, 128);                   
    createSolidGrayscalePNG(TEST5, 256, 256, 128);        
    createCheckerBoardPNG(TEST6, 128, 128, 16, 128);        

    // small image used to test
    createTestPNG1(TEST7, 4, 4, 200); 
}
