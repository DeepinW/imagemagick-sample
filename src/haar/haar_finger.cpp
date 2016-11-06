#include "haar_finger.h"

HaarFinger::HaarFinger():
    haar2d_(HAAR_POINT)
{
}

HaarFinger::~HaarFinger()
{

}

Geometry HaarFinger::GetTransformGeometry()
{
    char desc[64] = {0};
    snprintf(desc, sizeof(desc), "%dx%d!", MAT_SIZE, MAT_SIZE);
    return Geometry(desc);
}

bool HaarFinger::LoadImage(const char *image_path)
{
    Image image;

    try { 
        image.read(image_path);
        ReadPixelsAndTransform(&image);
    }
    catch (Exception &error) 
    { 
        return false; 
    }

    return true;
}

bool HaarFinger::LoadImage(const void *image_data, size_t size)
{

}

void HaarFinger::ReadPixelsAndTransform(Image *image)
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

    haar2d_.Transform(mat_);
}

void HaarFinger::GetFinger(uint32_t *finger_id, HaarFingerData *finger_data)
{
    for (int i = 0; i < MAT_SIZE; i++)
    {
        for (int j = 0; j < MAT_SIZE; j++)
        {
            (*finger_data)[MAT_SIZE * i + j] = 
                (int)((mat_[MAT_SIZE * i + j] / COLOR_RANGE) + 0.5);
        }
    }
    
    *finger_id = ((uint8_t)(*finger_data)[0] << 24) +
                 ((uint8_t)(*finger_data)[1] << 16) +
                 ((uint8_t)(*finger_data)[MAT_SIZE] << 8) +
                 ((uint8_t)(*finger_data)[MAT_SIZE + 1]);
}
