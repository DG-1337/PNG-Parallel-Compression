#include "../../include/filter.h"
#include "../../include/read_scan_line.h"

using namespace std;

void applyNoneFilter()
{
    return;
}

void applySubFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow)
{

    // edge cases 
    if (width < 2) return;                  // need at least 2 pixels for Sub filter

    size_t totalSize = image.size();
    size_t rowStart = 4 * currentRow * width;

    // makes sure that image has a valid byte width of 4 
    if (rowStart + width * 4 > totalSize) {
        cerr << "applySubFilter: row out of bounds (rowStart + width * 4 = "
             << rowStart + width * 4 << " > " << totalSize << ")" << endl;
        return;
    }

    // Copy original row to avoid in-place mutation issues
    vector<unsigned char> originalRow(image.begin() + rowStart, image.begin() + rowStart + width * 4);

    for(size_t i = 1; i < width; i++)
    {
        int dstIndex = rowStart + 4 * i;
        int srcIndex = 4 * (i - 1);

        image[dstIndex]     -= originalRow[srcIndex];           // red
        image[dstIndex + 1] -= originalRow[srcIndex + 1];       // green
        image[dstIndex + 2] -= originalRow[srcIndex + 2];       // blue
        image[dstIndex + 3] -= originalRow[srcIndex + 3];       // alpha
    }
}


void applyUpFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow)
{
    //can't apply up filter on first row of image
    if (currentRow == 0)
    {
        return;
    }

    //start at first pixel of row
    for(size_t i = 0; i < width; i++)
    {
        int index = 4 * (currentRow * width + i); //index of pixel in image vector

        //subtract pixel above's value from current pixel value for every channel
        image[index] -= image[index - (width * 4)]; //red channel
        image[index + 1] -= image[index - (width * 4) + 1]; //green channel
        image[index + 2] -= image[index - (width * 4) + 2]; //blue channel
        image[index + 3] -= image[index - (width * 4) + 3]; //alpha channel
    }
}


void applyAverageFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow)
{
    //can't apply average filter on first row of image
    if (currentRow == 0)
    {
        return;
    }

    //start at 2nd pixel of row
    for(size_t i = 1; i < width; i++)
    {
        int index = 4 * (currentRow * width + i); //index of pixel in image vector

        //calculate averages for every channel
        // avg = ((pixel to left) + (pixel above)) / 2
        int avgRed = (((image[index - 4]) + (image[index - (width * 4)])) / 2);
        int avgGreen = (((image[index - 3]) + (image[index - (width * 4) + 1])) / 2);
        int avgBlue = (((image[index - 2]) + (image[index - (width * 4) + 2])) / 2);
        int avgAlpha = (((image[index - 1]) + (image[index - (width * 4) + 3])) / 2);

        image[index] -= avgRed; //red channel
        image[index + 1] -= avgGreen; //green channel
        image[index + 2] -= avgBlue; //blue channel
        image[index + 3] -= avgAlpha; //alpha channel
    }
}

void applyPaethFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow)
{
    //can't apply average filter on first row of image
    if (currentRow == 0)
    {
        return;
    }

    //start at 2nd pixel of row
    for(size_t i = 1; i < width; i++)
    {
        int index = 4 * (currentRow * width + i);

        int leftIndex = index - 4;
        int aboveIndex = index - (width * 4);
        int aboveLeftIndex = index - (width * 4) - 4;

        //use paeth predictor on each channel
        int paethRed = paethPredictor(image[leftIndex], image[aboveIndex], image[aboveLeftIndex]);
        int paethGreen = paethPredictor(image[leftIndex + 1], image[aboveIndex + 1], image[aboveLeftIndex + 1]);
        int paethBlue = paethPredictor(image[leftIndex + 2], image[aboveIndex + 2], image[aboveLeftIndex + 2]);
        int paethAlpha = paethPredictor(image[leftIndex + 3], image[aboveIndex + 3], image[aboveLeftIndex + 3]);

        //apply predicted values
        image[index] -= paethRed;
        image[index + 1] -= paethGreen;
        image[index + 2] -= paethBlue;
        image[index + 3] -= paethAlpha;
    }
}

//helper function for paeth filter
//a = left pixel, b = above pixel, c = above left pixel
int paethPredictor(int a, int b, int c)
{
    //calculate values
    int p = a + b - c;
    int pa = abs(p - a);
    int pb = abs(p - b);
    int pc = abs(p - c);

    //pa is smallest
    if(pa <= pb && pa <= pc)
    {
        return a;
    }
    //pb is smallest
    else if(pb <= pc)
    {
        return b;
    }
    //pc is smallest
    else
    {
        return c;
    }
}

vector <unsigned char> applyFilteringMethod(const vector<unsigned char> image, unsigned w, unsigned h, int filterMethod) {
    vector <unsigned char> result; 

    if(image.size() != w * h * 4) {
        throw runtime_error("Error: image size does not match with expected size (w * h * 4)");
    }

    for (unsigned y = 0; y < h; ++y) {
        vector<unsigned char> row(image.begin() + y * w * 4, image.begin() + (y + 1) * w * 4); 
        applySubFilter(row, w, 0); 
        result.push_back(filterMethod);                                         // add filter type to start of scanline for decoding, sub: 1
        result.insert(result.end(), row.begin(), row.end());                    // append result with the filter row 
    }

    // sanity check 
    size_t expected_size = h * (1 + w * 4); 
    if(result.size() != expected_size) {
        throw runtime_error("Error: Filtered scanline != Original image"); 
    }

    return result; 
}