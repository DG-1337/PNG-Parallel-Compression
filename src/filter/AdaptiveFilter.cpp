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

    return imgData.image; 
}

// applys each filter and computes its score to decide which filter is best 
ImageData adaptiveFilter(ImageData imgdata) {

    // Initialize empty 0 vector with size of previous vector to store filtered scanlines 
    size_t vecSize = imgdata.image.size();
    vector<unsigned char> filteredVec(vecSize);               // used for debugging: stores saved filters 
    cout << "Filtering: " << endl; 

    for (size_t y = 0; y < imgdata.h; ++ y) {
        vector<unsigned char> bestScanline;         // saves best minimum scoring scanline 
        int bestFilter = -1;                        // save best minimum scoring filter
        int minScore = INT_MAX;                     // save best minimum score 
        size_t start_index = y * imgdata.w * 4; 

        for (int filterType = 0; filterType < 5; ++filterType){
            vector<unsigned char> filtered = applyFilters(imgdata, y, filterType); 
            int score = fastFilter_score(filtered);

            if (score < minScore) {
                minScore = score;                   // update minimum score 
                bestFilter = filterType;            // update filter type
                bestScanline = filtered;            // update scanline
            }
        }
        
        // iterates through best scanline from filters 
        for (size_t x = 0; x < imgdata.w; ++x) {
            size_t index = start_index + x * 4; 
            size_t filterIndex = x * 4; 

            bestScanline[index + 0] = filteredVec[filterIndex + 0];
            bestScanline[index + 1] = filteredVec[filterIndex + 1];
            bestScanline[index + 2] = filteredVec[filterIndex + 2];
            bestScanline[index + 3] = filteredVec[filterIndex + 3];

            cout << '(' <<
            (int)bestScanline[index + 0] << ',' <<
            (int)bestScanline[index + 1] << ',' <<
            (int)bestScanline[index + 2] << ',' <<
            (int)bestScanline[index + 3] << ") ";
        }

        cout << endl; 
    }

    ImageData filteredImage = {filteredVec, imgdata.w, imgdata.h, imgdata.fileName}; 
    return filteredImage;
}

void printFilteredImage(ImageData imgdata) {
    cout << "Post Filtering: " << endl; 

    for (size_t y = 0; y < imgdata.h; ++y) {
        size_t start_index = y * imgdata.w * 4; 
        for (size_t x = 0; x < imgdata.w; ++x){
            size_t index = start_index + x * 4; 
            cout << '(' <<
            (int)imgdata.image[index + 0] << ',' <<
            (int)imgdata.image[index + 1] << ',' <<
            (int)imgdata.image[index + 2] << ',' <<
            (int)imgdata.image[index + 3] << ") ";
        }
        cout << endl; 
    }
}

int main(int argc, char** argv) {
    string fileName = "tests/img/test7.png"; 
    ImageData imgdata = readScanLines(fileName);

    ImageData filteredImg = adaptiveFilter(imgdata);

    printFilteredImage(filteredImg);
    return 0; 
}
