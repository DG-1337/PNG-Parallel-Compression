#include <stdio.h>
#include "../../include/filter.h"
#include "../../include/ReadScanLine.h"
#include <filesystem>


using namespace std; 
struct ImageData;
struct Pixel; 

// fast approach to compute the filter score by summation 
int fastFilter_score(vector<unsigned char>& filteredScanline) {
    int score = 0; 

    for(size_t i = 1; i < filteredScanline.size(); ++i) {
        score += filteredScanline[i];
    }

    return score; 
}

// helper function to apply each subsequent filter 
vector<unsigned char> applyFilters(ImageData imgData, unsigned currRow, int currFilter) {
    
    switch (currFilter) {
        case 1:
            return imgData.image; 
        case 2:
            return applySubFilter(imgData.image, imgData.w, currRow); 
        case 3:
            return applyUpFilter(imgData.image, imgData.w, currRow); 
        case 4:
            return applyAverageFilter(imgData.image, imgData.w, currRow); 
        case 5:
            return applyPaethFilter(imgData.image, imgData.w, currRow); 
        default:
            break;
    }
}

// applys each filter and computes its score to decide which filter is best 
void adaptiveFilter(ImageData imgdata) {

    // Initialize empty 0 vector with size of previous vector to store filtered scanlines 
    size_t vecSize = imgdata.w * imgdata.h * 4;
    vector<unsigned char> filteredScanlines(vecSize);

    for (size_t y = 0; y < imgdata.h; ++ y) {
        vector<unsigned char> bestScanline; 
        int bestFilter = 0; 
        int minScore = INT_MAX; 
    }
}

int main(int argc, char** argv) {
    string fileName = "tests/img/test1.png"; 

    if (argc == 2) {
        fileName = argv[1];
    }

    ImageData imgdata = readScanLines(fileName); 
    return 0; 
}
