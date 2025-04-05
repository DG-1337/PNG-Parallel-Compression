#include <iostream>
#include <thread>
#include <vector>

#ifndef FILTER_H;
#define FILTER_H;

using namespace std;
int paethPredictor(int a, int b, int c);

void applyNoneFilter();

void applySubFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow);

void applyUpFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow);

void applyAverageFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow);

void applyPaethFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow);

int paethPredictor(int a, int b, int c);

vector <unsigned char> applyFilteringMethod(const vector<unsigned char> image, unsigned w, unsigned h, int filterMethod);


#endif;