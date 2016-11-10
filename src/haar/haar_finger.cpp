#include "haar_finger.h"
#include <cmath>

HaarTransform::HaarTransform():
    haar2d_(HAAR_POINT)
{
    int index = (int)(sizeof(region_) / sizeof(region_[0])) - 1;
    int size = MAT_SIZE;

    for (int iteration = 0; iteration < 3; iteration++)
    {
        size >>= 1;
        
        region_[index].x = size;
        region_[index].y = size;
        region_[index--].size = size;
        
        region_[index].x = 0;
        region_[index].y = size;
        region_[index--].size = size;

        region_[index].x = size;
        region_[index].y = 0;
        region_[index--].size = size;
    }

    assert(index == 0);

    region_[index].x = 0;
    region_[index].y = 0;
    region_[index].size = size;

}

HaarTransform::~HaarTransform()
{

}

Geometry HaarTransform::GetTransformGeometry()
{
    char desc[64] = {0};
    snprintf(desc, sizeof(desc), "%dx%d!", MAT_SIZE, MAT_SIZE);
    return Geometry(desc);
}

bool HaarTransform::LoadImage(const char *image_path)
{
    Image image;

    try { 
        image.read(image_path);
        ReadPixelsAndTransform(&image);
    }
    catch (Exception &error) 
    { 
        cerr << "Caught exception: " << error.what() << endl; 
        return false; 
    }

    return true;
}

bool HaarTransform::LoadImage(const void *image_data, size_t size)
{
    return false;
}

void HaarTransform::ReadPixelsAndTransform(Image *image)
{   
    static const Geometry size = GetTransformGeometry();

    image->resize(size);
    image->type(GrayscaleType);

    assert(image->rows() == MAT_SIZE);
    assert(image->columns() == MAT_SIZE);

    const PixelPacket *pixels = image->getConstPixels(0, 0, MAT_SIZE, MAT_SIZE);

    for (int i = 0; i < MAT_SIZE; i++)
    {
        for (int j = 0; j < MAT_SIZE; j++)
        {
            Color color = pixels[MAT_SIZE * i + j];
            mat_[MAT_SIZE * i + j] = (float)color.redQuantum();;
        }
    }

    haar2d_.Transform(mat_, 3);
}

void HaarTransform::GetFinger(HaarFinger* finger)
{
    for (size_t i = 0; i < sizeof(region_) / sizeof(region_[0]); i++)
    {
        finger->intensity[i] = GetIntensity(region_[i]);
    }
}

bool MatchHaarFinger(const HaarFinger &src, const HaarFinger &target, float max_ratio)
{
    float diff = 0.0f;
    float ratio = 0.0f;

    for (size_t i = 0; i < sizeof(src.intensity) / sizeof(target.intensity[0]); i++)
    {
        diff = fabs(src.intensity[i] - target.intensity[i]);
        if (src.intensity[i] != 0.0f)
        {
            ratio = diff / abs(src.intensity[i]);
        }
        else
        {
            ratio = INFINITY;
        }

        if (ratio > max_ratio)
        {
            return false;
        }
    }

    return true;
}

float HaarTransform::GetIntensity(const Region &region)
{
    assert(region.size > 0);
    assert(region.x >= 0 && region.x + region.size <= MAT_SIZE);
    assert(region.y >= 0 && region.y + region.size <= MAT_SIZE);

    float sum_power = 0.0f;

    for (int i = region.x; i < region.x + region.size; i++)
    {
        for (int j = region.y; j < region.y + region.size; j++)
        {
            sum_power += (mat_[i * MAT_SIZE + j] * mat_[i * MAT_SIZE + j]);
        }
    }

    return sqrt(sum_power / (region.size * region.size));
}

//gzrd_Lib_CPP_Version_ID--start
#ifndef GZRD_SVN_ATTR
#define GZRD_SVN_ATTR "0"
#endif
static char gzrd_Lib_CPP_Version_ID[] __attribute__((used))="$HeadURL$ $Id$ " GZRD_SVN_ATTR "__file__";
// gzrd_Lib_CPP_Version_ID--end

