#include "../libs/lodepng.h"  // Use double quotes for local files
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

using namespace std; 

struct Pixel {
    unsigned r, g, b, a;
};

// reads a png line by line and returns its rbga pixel values in a 2d vector representing the image 
vector<vector<Pixel> > mt_readScanLines(const string fileName) {
    vector<unsigned char> image;
    unsigned w, h;

    unsigned err = lodepng::decode(image, w, h, fileName);
    if (err) {
        cout << "Decoding error " << err << ": " << lodepng_error_text(err) << endl;
        return vector<vector<Pixel> >();
    }

    vector<vector<Pixel> > pixels(h, vector<Pixel>(w));                 // Pre-allocate
    atomic<unsigned> next_row(0); 
    const unsigned NUM_THREADS = thread::hardware_concurrency();        // automatically get number of threads 
    vector<thread> threads; 


    
    return pixels;
}


