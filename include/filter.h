#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void applyNoneFilter()
{
    return;
}

void applySubFilter(vector<unsigned char>& image, unsigned width, unsigned currentRow)
{
    //start from second pixel of row
    for(size_t i = 1; i < width; i++)
    {
        int index = 4 * (currentRow * width + i); //index of pixel in image vector

        //subtract previous pixel's value from current pixel value for every channel
        image[index] -= image[index - 4]; //red channel
        image[index + 1] -= image[index - 3]; //green channel
        image[index + 2] -= image[index - 2]; //blue channel
        image[index + 3] -= image[index - 1]; //alpha channel
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