#include <Magick++.h> 
#include <iostream>
#include <cassert>
#include <cstring>

using namespace std; 
using namespace Magick; 

const int COLOR_DEPTH = 64;
const int HAAR_LEVEL = 6;
const int MATRIX_SIZE = 1 << HAAR_LEVEL;

void UsageExit(const char *cmd)
{
    printf("Usage:\n  %s <image_path>\n", cmd);
    exit(1);
}

inline void InPlaceHaarRow(int n, int row_num, float *matrix)
{
    assert(n > 0);
    assert(n <= HAAR_LEVEL);

    static float buff[MATRIX_SIZE];

    float *a = &buff[0];
    float *d = &buff[1 << (n - 1)];

    float *row = matrix + (row_num * MATRIX_SIZE);

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        a[i] = (row[i * 2] + row[i * 2 + 1]) / 2;
        d[i] = (row[i * 2] - row[i * 2 + 1]) / 2; 
    }

    memcpy(row, buff, sizeof(float) * (1 << n));
}

inline void InPlaceHaarCol(int n, int col_num, float *matrix)
{
    assert(n > 0);
    assert(n <= HAAR_LEVEL);

    static float buff[MATRIX_SIZE];

    float *a = &buff[0];
    float *d = &buff[1 << (n - 1)];

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        a[i] = (matrix[i * 2 * MATRIX_SIZE + col_num] + matrix[(i * 2 + 1) * MATRIX_SIZE + col_num]) / 2;
        d[i] = (matrix[i * 2 * MATRIX_SIZE + col_num] - matrix[(i * 2 + 1) * MATRIX_SIZE + col_num]) / 2;
    }

    for (int i = 0; i < (1 << (n - 1)); i++)
    {
        matrix[i * MATRIX_SIZE + col_num] = a[i];
        matrix[((1 << (n - 1)) + i) * MATRIX_SIZE + col_num] = d[i];
    }
}

inline void InPlaceHaar2D(int n, float *matrix)
{
    while (n > 0)
    {
        for (int i = 0; i < (1 << n); i++)
        {
            InPlaceHaarRow(n, i, matrix);
        }

        for (int i = 0; i < (1 << n); i++)
        {
            InPlaceHaarCol(n, i, matrix);
        }

        n--;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        UsageExit(argv[0]);
    }

    InitializeMagick(*argv);

    const char *image_path = argv[1];

    Image image;
    char size_desc[64] = {0};
    snprintf(size_desc, sizeof(size_desc), "%dx%d!", MATRIX_SIZE, MATRIX_SIZE);

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

    assert(w == MATRIX_SIZE);
    assert(h == MATRIX_SIZE);

    float matrix[MATRIX_SIZE * MATRIX_SIZE];
    int value = 0;

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            Color color = pixels[w * i + j];
            // value = color.redQuantum() / (65536 / COLOR_DEPTH);
            value = color.redQuantum();
            matrix[MATRIX_SIZE * i + j] = (float)value;
        }
    }

    InPlaceHaar2D(HAAR_LEVEL, matrix);

    for (int i = 0; i < MATRIX_SIZE; i++)
    {
        for (int j = 0; j < MATRIX_SIZE; j++)
        {
            // printf("%d ", (int)matrix[MATRIX_SIZE * i + j]);
            // printf("%.02f ", matrix[MATRIX_SIZE * i + j]);
            printf("%+03d ", (int)((matrix[MATRIX_SIZE * i + j] / (65536 / COLOR_DEPTH)) + 0.5));
        }

        printf("\n");
    }

    return 0;
}
