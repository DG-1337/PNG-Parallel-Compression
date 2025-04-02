#include "../../include/read_scan_line.h"

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

// Get single scanline r g b a values given its row and width of image in the image vector
vector<unsigned char> getSingleScanline(vector<unsigned char> image, unsigned w, unsigned currRow) {

    vector<unsigned char> scanline((size_t)w * 4);          

    for (size_t i = 0; i < w; ++i) {
        int imageIndex = 4 * (currRow * w + i);             // index of scanline located in complete image vector
        int scanIndex = 4 * i;                              // index of new single scanline vector 
        
        scanline[scanIndex + 0] = image[imageIndex + 0];    // red channel
        scanline[scanIndex + 1] = image[imageIndex + 1];    // green channel
        scanline[scanIndex + 2] = image[imageIndex + 2];    // blue channel 
        scanline[scanIndex + 3] = image[imageIndex + 3];    // alpha channel
    }
    return scanline;
}