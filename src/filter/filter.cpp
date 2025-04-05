#include "../../include/filter.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

//helper function for paeth filter
//a = left pixel, b = above pixel, c = above left pixel
int paethPredictor(int a, int b, int c)
{
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);

    if(pa <= pb && pa <= pc)
    {
        return a;
    }
    else if(pb <= pc)
    {
        return b;
    }
    else
    {
        return c;
    }
}

// None filter just returns the row with filter byte 0
vector<unsigned char> applyNoneFilter(const vector<unsigned char>& row)
{
    vector<unsigned char> result;
    result.push_back(0);                      // filter type: None
    result.insert(result.end(), row.begin(), row.end());
    return result;
}

// Sub filter: current pixel - pixel to the left
vector<unsigned char> applySubFilter(const vector<unsigned char>& row, unsigned width)
{
    vector<unsigned char> result(row.size() + 1);
    result[0] = 1;                            // filter type: Sub

    for (unsigned i = 0; i < width; i++)
    {
        int index = 4 * i;

        for (int channel = 0; channel < 4; ++channel)
        {
            int left = (i > 0) ? row[index + channel - 4] : 0;
            result[1 + index + channel] = row[index + channel] - left;
        }
    }

    return result;
}

// Up filter: current pixel - pixel above
vector<unsigned char> applyUpFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow)
{
    vector<unsigned char> result(row.size() + 1);
    result[0] = 2;                            // filter type: Up

    for (size_t i = 0; i < row.size(); ++i)
    {
        int above = prevRow.empty() ? 0 : prevRow[i];
        result[1 + i] = row[i] - above;
    }

    return result;
}

// Average filter: current pixel - average(left, above)
vector<unsigned char> applyAverageFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned width)
{
    vector<unsigned char> result(row.size() + 1);
    result[0] = 3;                            // filter type: Average

    for (unsigned i = 0; i < width; i++)
    {
        int index = 4 * i;

        for (int channel = 0; channel < 4; ++channel)
        {
            int left = (i > 0) ? row[index + channel - 4] : 0;
            int above = prevRow.empty() ? 0 : prevRow[index + channel];
            int avg = (left + above) / 2;
            result[1 + index + channel] = row[index + channel] - avg;
        }
    }

    return result;
}

// Paeth filter: current pixel - paethPredictor(left, above, upperLeft)
vector<unsigned char> applyPaethFilter(const vector<unsigned char>& row, const vector<unsigned char>& prevRow, unsigned width)
{
    vector<unsigned char> result(row.size() + 1);
    result[0] = 4;                            // filter type: Paeth

    for (unsigned i = 0; i < width; i++)
    {
        int index = 4 * i;

        for (int channel = 0; channel < 4; ++channel)
        {
            int left      = (i > 0) ? row[index + channel - 4] : 0;
            int above     = prevRow.empty() ? 0 : prevRow[index + channel];
            int upperLeft = (i > 0 && !prevRow.empty()) ? prevRow[index + channel - 4] : 0;

            int paeth = paethPredictor(left, above, upperLeft);
            result[1 + index + channel] = row[index + channel] - paeth;
        }
    }

    return result;
}
