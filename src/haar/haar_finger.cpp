#include "haar_finger.h"

HaarTransform::HaarTransform():
    haar2d_(HAAR_POINT)
{
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
        cout << "Caught exception: " << error.what() << endl; 
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

    haar2d_.Transform(mat_);
}

void HaarTransform::GetFinger(HaarFinger* finger)
{
    for (int i = 0; i < MAT_SIZE; i++)
    {
        for (int j = 0; j < MAT_SIZE; j++)
        {
            finger->data[MAT_SIZE * i + j] = 
                (uint8_t)((mat_[MAT_SIZE * i + j] / COLOR_RANGE) + 0.5);
        }
    }
    
    finger->id = (finger->data[0] << 24) +
                 (finger->data[1] << 16) +
                 (finger->data[MAT_SIZE] << 8) +
                 (finger->data[MAT_SIZE + 1]);


    finger->accerator[0] = finger->data[2];
    finger->accerator[1] = finger->data[3];
    finger->accerator[2] = finger->data[MAT_SIZE + 2];
    finger->accerator[3] = finger->data[MAT_SIZE + 3];

    finger->accerator[4] = finger->data[MAT_SIZE * 2];
    finger->accerator[5] = finger->data[MAT_SIZE * 2 + 1];
    finger->accerator[6] = finger->data[MAT_SIZE * 3];
    finger->accerator[7] = finger->data[MAT_SIZE * 3 + 1];

    finger->accerator[8] = finger->data[MAT_SIZE * 2 + 2];
    finger->accerator[9] = finger->data[MAT_SIZE * 2 + 3];
    finger->accerator[10] = finger->data[MAT_SIZE * 3 + 2];
    finger->accerator[11] = finger->data[MAT_SIZE * 3 + 3];
}

bool MatchHaarFinger(const HaarFinger &a, const HaarFinger &b, HaarFingerDiff *diff)
{
    memset(diff, 0, sizeof(*diff));

    if (a.id != b.id)
    {
        diff->id_diff = true;
        return false;
    }

    for (int i = 0; i < sizeof(a.accerator); i++)
    {
        if (a.accerator[i] != b.accerator[i])
        {
            ++(diff->accerator_diff_cnt);
        }
    }

    if (diff->accerator_diff_cnt > 3)
    {
        return false;
    }

    for (int i = 0; i < sizeof(a.data); i++)
    {
        if (a.data[i] != b.data[i])
        {
            ++(diff->data_diff_cnt);
        }

        if (a.data[i] != 0 || b.data[i] != 0)
        {
            ++(diff->data_valid_cnt);
        }
    }

    if (diff->data_valid_cnt == 0 || diff->data_diff_cnt == 0)
    {
        diff->match_confidence = 1.0f;
    }
    else
    {
        diff->match_confidence = (float)diff->data_diff_cnt / (float)diff->data_valid_cnt;
    }

    return true;
}

//gzrd_Lib_CPP_Version_ID--start
#ifndef GZRD_SVN_ATTR
#define GZRD_SVN_ATTR "0"
#endif
static char gzrd_Lib_CPP_Version_ID[] __attribute__((used))="$HeadURL$ $Id$ " GZRD_SVN_ATTR "__file__";
// gzrd_Lib_CPP_Version_ID--end

