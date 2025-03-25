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
        score += abs(filteredScanline[i]);
    }

    return score; 
}

// helper function to apply each subsequent filter 
vector<unsigned char> applyFilters(ImageData imgdata, int currFilter) {

}

// applys each filter and computes its score to decide which filter is best 
void adaptiveFilter(ImageData imgdata) {
    vector<unsigned char> bestScanline; 
    int bestFilter = 0; 
    int minScore = INT_MAX; 
}

int main(int argc, char** argv) {
    string fileName = "tests/img/test1.png"; 

    if (argc == 2) {
        fileName = argv[1];
    }

    ImageData imgdata = readScanLines(fileName); 
    return 0; 
}
