#include <iostream>
#include <thread>
#include <vector>

#ifndef FILTER_H;
#define FILTER_H;

using namespace std;
int paethPredictor(int a, int b, int c);

vector<unsigned char> applyNoneFilter(const vector<unsigned char>& row);

vector<unsigned char> applySubFilter(const vector<unsigned char>& row, unsigned width);

vector<unsigned char> applyUpFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow);

vector<unsigned char> applyAverageFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned width);

vector<unsigned char> applyPaethFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned width);

#endif;