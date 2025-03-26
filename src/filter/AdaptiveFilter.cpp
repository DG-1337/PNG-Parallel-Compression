#include <stdio.h>
#include "../../include/filter.h"
#include "../../include/ReadScanLine.h"
#include <filesystem>
#include <algorithm>

using namespace std; 
struct ImageData;
struct Pixel; 

void printScanLine(vector<unsigned char> scanline, int filterMethod){
    cout << "Filter Method: " << filterMethod << endl; 
    for (size_t i = 0; i < scanline.size(); ++i){
        size_t index = i * 4; 
        cout << '(' <<
        (int)scanline[index + 0] << ',' <<
        (int)scanline[index + 1] << ',' <<
        (int)scanline[index + 2] << ',' <<
        (int)scanline[index + 3] << ") ";
    }
    cout << endl; 
    
}

// fast approach to compute the filter score by summation 
int fastFilter_score(vector<unsigned char> filteredScanline) {
    int score = 0; 

    for(size_t i = 1; i < filteredScanline.size(); ++i) {
        score += filteredScanline[i];
    }

    return score; 
}

// helper function to apply each subsequent filter 
void applyFilters(ImageData imgData, unsigned currRow, int currFilter) {
    
    switch (currFilter) {
        case 0:
            applyNoneFilter(); 
            break; 
        case 1:
            applySubFilter(imgData.image, imgData.w, currRow); 
            break;
        case 2:
            applyUpFilter(imgData.image, imgData.w, currRow); 
            break; 
        case 3:
            applyAverageFilter(imgData.image, imgData.w, currRow); 
            break; 
        case 4:
            applyPaethFilter(imgData.image, imgData.w, currRow); 
            break;
        default:
            break;
    }

    return;
}

// applys each filter and computes its score to decide which filter is best 
ImageData adaptiveFilter(ImageData imgdata) {

    // Initialize empty 0 vector with size of previous vector to store filtered scanlines 
    size_t vecSize = imgdata.image.size();
    vector<unsigned char> filteredVec(vecSize);     
    cout << "Filtering: " << endl; 

    for (size_t y = 0; y < imgdata.h; ++ y) {
        vector<unsigned char> bestScanline;         // saves best minimum scoring scanline 
        int bestFilter = -1;                        // save best minimum scoring filter
        int minScore = INT_MAX;                     // save best minimum score 
        size_t start_index = y * imgdata.w * 4; 

        for (int filterType = 0; filterType < 5; ++filterType){
            vector<unsigned char> filtered(size_t(imgdata.w));
            applyFilters(imgdata, y, filterType); 

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

            filteredVec[index + 0] = bestScanline[filterIndex + 0];
            filteredVec[index + 1] = bestScanline[filterIndex + 1];
            filteredVec[index + 2] = bestScanline[filterIndex + 2];
            filteredVec[index + 3] = bestScanline[filterIndex + 3];

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

    return 0; 
}
