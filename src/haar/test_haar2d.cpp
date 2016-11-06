#include "haar2d.h"

#include <Magick++.h> 
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std; 
using namespace Magick; 

const int COLOR_DEPTH = 64;
const int HAAR_POINT = 6;

void UsageExit(const char *cmd)
{
    printf("Usage:\n  %s <image_path>\n", cmd);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        UsageExit(argv[0]);
    }

    InitializeMagick(*argv);
    Haar2D<float> haar2d(HAAR_POINT);

    const int mat_size = 1 << HAAR_POINT;

    const char *image_path = argv[1];

    Image image;
    char size_desc[64] = {0};
    snprintf(size_desc, sizeof(size_desc), "%dx%d!", mat_size, mat_size);

    try { 
        image.read(image_path);
        Geometry size("64x64!");
        image.resize(size);
        image.type(GrayscaleType);
        image.write(string(image_path) + ".256.jpeg");
    } 
    catch (Exception &error) 
    { 
        cout << "Caught exception: " << error.what() << endl; 
        return 1; 
    }


    int w = image.columns();
    int h = image.rows();

    const PixelPacket *pixels = image.getConstPixels(0, 0, w, h);

    printf("w = %d h = %d\n", w, h);

    assert(w == mat_size);
    assert(h == mat_size);

    float mat[mat_size * mat_size];
    int value = 0;

    for (int i = 0; i < mat_size; i++)
    {
        for (int j = 0; j < mat_size; j++)
        {
            Color color = pixels[w * i + j];
            // value = color.redQuantum() / (65536 / COLOR_DEPTH);
            value = color.redQuantum();
            mat[mat_size * i + j] = (float)value;
        }
    }


    haar2d.Transform(mat);

    for (int i = 0; i < mat_size; i++)
    {
        for (int j = 0; j < mat_size; j++)
        {
            // printf("%d ", (int)mat[mat_size * i + j]);
            // printf("%.02f ", mat[mat_size * i + j]);
            printf("%+03d ", (int)((mat[mat_size * i + j] / (65536 / COLOR_DEPTH)) + 0.5));
        }

        printf("\n");
    }

    return 0;
}
