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

void printScanLine(vector<unsigned char> scanline, unsigned w, int filterMethod);
int fastFilter_score(const vector<unsigned char>& filteredScanline);
vector<unsigned char> applyFilterMethod(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned w, int filterMethod);
ImageData adaptiveFilter(ImageData imgdata);
void printImage(vector<unsigned char> pixels, unsigned w, unsigned h);

#endif;