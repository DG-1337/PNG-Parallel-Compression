#include "../libs/lodepng.h" 
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>

using namespace std; 

struct Pixel {
    unsigned r, g, b, a;
};

struct ImageData {
    vector<unsigned char> image; 
    unsigned w, h; 
    const string fileName; 
};

// Single vector approach
ImageData readScanLines(const string fileName) {
    vector<unsigned char> image;            // vector containing r, g, b, a values for each pixel 
    unsigned w, h;                          // width and height 

    unsigned err = lodepng::decode(image, w, h, fileName); 
    if (err) {
        cout << "Decoding error" << err << ":" << lodepng_error_text(err) << endl; 
    }

    ImageData imgData = {image, w, h, fileName}; 
    return imgData;
}


// Multi-threaded implementation, reads an image file and gets (r, b, g, a) values of a given image 
vector<vector<Pixel> > mt_readScanLines(const string fileName) {
    vector<unsigned char> image;            // vector containing r, g, b, a values for each pixel 
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

    // lambda worker function to process each row from the image 
    auto processScanLines = [&]() {
        while (true) {
            unsigned y = next_row.fetch_add(1);
            if(y >= h) {
                break; 
            }
            
            // places image pixel data back into a vector to better organize the data with type Pixel 
            for (unsigned x = 0; x < w; x++) {
                unsigned index = (y * w  + x) * 4; 
                pixels[y][x] = {image[index], image[index + 1], image[index + 2], image[index + 3]};    
            }
        }
    };

    for (unsigned i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(processScanLines); 
    }

    for (auto& t: threads) {
        t.join(); 
    }
    
    return pixels;
}

// Single-threaded implementation, reads an image file and gets (r, b, g, a) values of a given image 
vector<vector<Pixel> > st_readScanLines(const string fileName) {
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
        unsigned start_index = y * w * 4;

        // iterates over the each pixel for a row 
        for (unsigned x = 0; x < w; x++) {
            unsigned index = start_index + x * 4;
            Pixel p = {image[index], image[index + 1], image[index + 2], image[index + 3]};
            pixels[y][x] = p; 
        }
    }
    
    return pixels;
}