#include "filter.h"
#include "read_scan_line.h"
#include <filesystem>
#include <algorithm>
#include <stdio.h>
#include <vector>


#ifndef ADAPTIVE_FILTER_H;
#define ADAPTIVE_FILTER_H;

using namespace std;
struct ImageData;
struct Pixel; 

void printScanLine(vector<unsigned char> scanline, unsigned w, int filterMethod = -1);
int fastFilter_score(vector<unsigned char> filteredScanline, unsigned w);
void applyFilters(vector<unsigned char> &img, unsigned w, unsigned currRow, int currFilter);
ImageData adaptiveFilter(ImageData imgdata);
void printImage(vector<unsigned char> pixels, unsigned w, unsigned h);

#endif;