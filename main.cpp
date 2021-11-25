#include <iostream>
#include <CImg.h>

using namespace cimg_library;

int main() {
    const int xmax = 200;
    const int ymax = 100;
    CImg<float> img(xmax, ymax, 1, 3, 0);
    const unsigned char red[] = {255, 0, 0}, green[] = {0, 255, 0}, blue[] = {0, 0, 255};
    for (int x = 0; x < xmax; x++) {
        for (int y = 0; y < ymax; y++) {
            img(x,y,0, 0) = (float)x / (float)xmax * 255;
            img(x,y,0, 1) = (float)y / (float)ymax * 255;
            img(x,y,0, 2) = 255.0;
        }
    }
    img.save("test.exr");
    return 0;
}

