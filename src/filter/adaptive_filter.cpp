#include "../../include/adaptive_filter.h"

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
int fastFilter_score(vector<unsigned char> filteredScanline, unsigned w) {
    int score = 0; 
    for(size_t i = 0; i < w; ++i) {
        // Gets location of pixel values for scanline 
        score += abs(filteredScanline[i] - filteredScanline[i - 1]);
    }
    return score; 
}

// helper function to apply each subsequent filter 
void applyFilters(vector<unsigned char> &img, unsigned w, unsigned currRow, int currFilter) {
    
    switch (currFilter) {
        case 0:
            applyNoneFilter(); 
            break; 
        case 1:
            applySubFilter(img, w, currRow); 
            break;
        case 2:
            applyUpFilter(img, w, currRow); 
            break; 
        case 3:
            applyAverageFilter(img, w, currRow); 
            break; 
        case 4:
            applyPaethFilter(img, w, currRow); 
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

    for (size_t y = 0; y < imgdata.h; ++ y) {
        vector<unsigned char> bestScanline;         // saves best minimum scoring scanline 
        int bestFilter = -1;                        // save best minimum scoring filter
        int minScore = INT_MAX;                     // save best minimum score 
        size_t start_index = y * imgdata.w * 4;   

         // gets curr scanline to apply filters for
        vector<unsigned char> currScanline = getSingleScanline(imgdata.image, imgdata.w, y);       

        // Applies each and every filter 
        for (int filterType = 0; filterType < 5; ++filterType){
            vector<unsigned char> filteredScanline = currScanline;
            applyFilters(filteredScanline, imgdata.w, y, filterType); 
            
            int score = fastFilter_score(filteredScanline, imgdata.w);
            if (score < minScore) {
                minScore = score;                           // update minimum score 
                bestFilter = filterType;                    // update filter type
                bestScanline = filteredScanline;            // update scanline
            }
        }

        // iterates through best scanline from filters and places values in vector 
        for (size_t x = 0; x < imgdata.w; ++x) {
            size_t index = start_index + x * 4; 
            size_t filterIndex = x * 4; 

            filteredVec[index + 0] = bestScanline[filterIndex + 0];
            filteredVec[index + 1] = bestScanline[filterIndex + 1];
            filteredVec[index + 2] = bestScanline[filterIndex + 2];
            filteredVec[index + 3] = bestScanline[filterIndex + 3];
        }
    }

    // creates image struct/object and returns it 
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
