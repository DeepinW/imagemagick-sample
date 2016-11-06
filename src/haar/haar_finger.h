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

typedef int HaarFingerData[MAT_SIZE * MAT_SIZE];

class HaarFinger
{
public:

    HaarFinger();

    ~HaarFinger();

    bool LoadImage(const char *image_path);

    bool LoadImage(const void *image_data, size_t size);

    void GetFinger(uint32_t *finger_id, HaarFingerData *finger_data);

private:

    void ReadPixelsAndTransform(Image *image);
    static Geometry GetTransformGeometry();

private:

    Haar2D<float> haar2d_;
    float mat_[MAT_SIZE * MAT_SIZE];

};


#endif

