#include "../../include/adaptive_filter.h"
#include "../../include/filter.h"

void printScanLine(vector<unsigned char> scanline, unsigned w, int filterMethod){
    if(filterMethod > -1){
        cout << "Filter Method: " << filterMethod << endl; 
    }

    for (size_t i = 0; i < w; ++i){
        size_t index = i * 4; 
        cout << '(' <<
        (int)scanline[index + 0] << ',' <<
        (int)scanline[index + 1] << ',' <<
        (int)scanline[index + 2] << ',' <<
        (int)scanline[index + 3] << ") ";
    }
    cout << endl; 
}

// fast approach to compute the filter score by summation of absolute difference 
int fastFilter_score(const vector<unsigned char>& filteredScanline) {
    int score = 0; 
    for(size_t i = 2; i < filteredScanline.size(); ++i) {
        score += abs(filteredScanline[i] - filteredScanline[i - 1]);
    }
    return score; 
}

// helper function to apply a single filter using new modular methods
vector<unsigned char> applyFilterMethod(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned w, int filterMethod) {
    switch (filterMethod) {
        case 0:
            return applyNoneFilter(row); 
        case 1:
            return applySubFilter(row, w); 
        case 2:
            return applyUpFilter(row, prevRow); 
        case 3:
            return applyAverageFilter(row, prevRow, w); 
        case 4:
            return applyPaethFilter(row, prevRow, w); 
        default:
            throw runtime_error("Unknown filter method in applyFilterMethod");
    }
}

// applys each filter and computes its score to decide which filter is best 
ImageData adaptiveFilter(ImageData imgdata) {

    // Initialize result vector to hold all filtered scanlines + filter bytes
    vector<unsigned char> filteredVec;
    filteredVec.reserve(imgdata.h * (1 + imgdata.w * 4));

    for (size_t y = 0; y < imgdata.h; ++y) {
        vector<unsigned char> currScanline = getSingleScanline(imgdata.image, imgdata.w, y);
        vector<unsigned char> prevScanline;

        if (y > 0) {
            prevScanline = getSingleScanline(imgdata.image, imgdata.w, y - 1);
        }

        vector<unsigned char> bestScanline;
        int bestFilter = -1;
        int minScore = INT_MAX;

        // Try all 5 filters
        for (int filterType = 0; filterType < 5; ++filterType) {
            vector<unsigned char> filteredScanline = applyFilterMethod(currScanline, prevScanline, imgdata.w, filterType);

            int score = fastFilter_score(filteredScanline);
            if (score < minScore) {
                minScore = score;
                bestFilter = filterType;
                bestScanline = filteredScanline;
            }
        }

        // Add chosen filter scanline to final filtered image
        filteredVec.insert(filteredVec.end(), bestScanline.begin(), bestScanline.end());
    }

    // create image struct/object and return it 
    ImageData filteredImage = {filteredVec, imgdata.w, imgdata.h, imgdata.fileName}; 
    return filteredImage;
}

void printImage(vector<unsigned char> pixels, unsigned w, unsigned h) {
    for (size_t y = 0; y < h; y++) {
        size_t start_index = y * w * 4; 
        for (size_t x = 0; x < w; x++) {
            size_t index = start_index + x * 4; 
            cout << "(" 
            << (int)pixels[index + 0] << "," 
            << (int)pixels[index + 1] << "," 
            << (int)pixels[index + 2] << "," 
            << (int)pixels[index + 3] << ") ";
        }
        cout << endl; 
    }
}
