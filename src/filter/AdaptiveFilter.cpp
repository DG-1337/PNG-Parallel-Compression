#include <stdio.h>
#include "../../include/filter.h"
#include "../../include/ReadScanLine.h"
#include <filesystem>
#include <algorithm>

using namespace std; 
struct ImageData;
struct Pixel; 

// fast approach to compute the filter score by summation 
int fastFilter_score(vector<unsigned char> filteredScanline) {
    int score = 0; 

    for(size_t i = 1; i < filteredScanline.size(); ++i) {
        score += filteredScanline[i];
    }

    return score; 
}

// helper function to apply each subsequent filter 
vector<unsigned char> applyFilters(ImageData imgData, unsigned currRow, int currFilter) {
    
    switch (currFilter) {
        case 0:
            return imgData.image; 
        case 1:
            return applySubFilter(imgData.image, imgData.w, currRow); 
        case 2:
            return applyUpFilter(imgData.image, imgData.w, currRow); 
        case 3:
            return applyAverageFilter(imgData.image, imgData.w, currRow); 
        case 4:
            return applyPaethFilter(imgData.image, imgData.w, currRow); 
        default:
            break;
    }
}

// applys each filter and computes its score to decide which filter is best 
void adaptiveFilter(ImageData imgdata) {

    // Initialize empty 0 vector with size of previous vector to store filtered scanlines 
    vector<vector<unsigned char>> filteredScanlines(imgdata.h * 4, vector<unsigned char>(imgdata.w * 4));

    vector<int> bestFilter(imgdata.h);               // used for debugging: stores saved filters 

    for (size_t y = 0; y < imgdata.h; ++ y) {
        vector<unsigned char> bestScanline;         // saves best minimum scoring scanline 
        int bestFilter = -1;                        // save best minimum scoring filter
        int minScore = INT_MAX;                     // save best minimum score 

        for (int filterType = 0; filterType < 5; ++filterType){
            vector<unsigned char> filtered = applyFilters(imgdata, y, filterType); 
            int score = fastFilter_score(filtered);

            if (score < minScore) {
                minScore = score;                   // update minimum score 
                bestFilter = filterType;            // update filter type
                bestScanline = filtered;            // update scanline
            }
        }
        filteredScanlines[y] = bestScanline; 
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
