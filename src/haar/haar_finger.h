#ifndef _HAAR_FINGER_H_INCLUDED_
#define _HAAR_FINGER_H_INCLUDED_

#include "haar2d.h"
#include <Magick++.h> 
#include <iostream>
#include <cassert>
#include <cstring>
#include <stdint.h>

using namespace std; 
using namespace Magick; 

const int COLOR_DEPTH = 64;
const int HAAR_POINT = 6;

const int COLOR_RANGE = 65536 / COLOR_DEPTH;
const int MAT_SIZE = 1 << HAAR_POINT;

struct HaarFinger
{
    uint32_t id;
    uint8_t accerator[12];
    uint8_t data[MAT_SIZE * MAT_SIZE];
};

struct HaarFingerDiff
{
    bool id_diff;
    uint32_t accerator_diff_cnt;
    uint32_t data_diff_cnt;
    uint32_t data_valid_cnt;
    float match_confidence;
};


bool MatchHaarFinger(const HaarFinger &a, const HaarFinger &b, HaarFingerDiff *diff);

class HaarTransform
{
public:

    HaarTransform();

    ~HaarTransform();

    bool LoadImage(const char *image_path);

    bool LoadImage(const void *image_data, size_t size);

    void GetFinger(HaarFinger *finger);

private:

    void ReadPixelsAndTransform(Image *image);
    static Geometry GetTransformGeometry();

private:

    Haar2D<float> haar2d_;
    float mat_[MAT_SIZE * MAT_SIZE];

};


#endif

