#include <stdio.h>
#include "../../include/filter.h"
#include "../../include/ReadScanLine.h"
#include <filesystem>


using namespace std; 
struct ImageData;
struct Pixel; 

// fastest approach computes the sum of absolute differences in a filtered scanline 
int fast_filter_score() {
    int score = 0; 
}

int main(int argc, char** argv) {
    string fileName = "tests/img/test1.png"; 

    if (argc == 2) {
        fileName = argv[1];
    }

    ImageData imgdata = readScanLines(fileName); 
    vector<vector<Pixel> > pixels = mt_readScanLines(fileName); 


    return 0; 
}
