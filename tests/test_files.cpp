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
            image[index] = x * w;
            image[index + 1] = y * 16;
            image[index + 2] = 128;
            image[index + 3] = 255;
        }
    }

    unsigned err = lodepng::encode(fileName, image, w, h);
    if (err) {
        cout << "Encoding error " << err << ": " << lodepng_error_text(err) << endl;
    } else {
        cout << "Image saved as " << fileName << endl;
    }
}

int main() {
    const string test1 = "tests/img/test1.png"; 
    unsigned w1 = 16; 
    unsigned h1 = 16; 

    createTestPNG1(test1, h1, w1); 

    return 0; 
}
