#include <iostream>
#include <thread>
#include <zlib.h>
#include <mutex>
#include <vector>

#ifndef COMPRESSION_H;
#define COMPRESSION_H;

using namespace std;

vector<unsigned char> compress_image(const vector<unsigned char>& input);

#endif;