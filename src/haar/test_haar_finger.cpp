#include "haar_finger.h"

int main(int argc, char *argv[])
{
    const char *image_path = argv[1];

    HaarFinger haar_finger;

    if (!haar_finger.LoadImage(image_path))
    {
        printf("Load image fail: %s\n", image_path);
        return 1;
    }

    uint32_t finger_id;
    HaarFingerData finger_data;
    memset(&finger_data, 0, sizeof(finger_data));

    haar_finger.GetFinger(&finger_id, &finger_data);

    printf("finger_id = %x\n", finger_id);

    return 0;
}
