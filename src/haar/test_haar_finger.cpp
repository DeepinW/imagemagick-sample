#include "haar_finger.h"

void UsageExit(const char *cmd)
{
    printf("Usage:\n  %s <src_image_path> <image_path1> [image_path2...]\n", cmd);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        UsageExit(argv[0]);
    }

    InitializeMagick(*argv);

    const char *src_image_path = argv[1];

    const char * const *image_path = &argv[2];
    int image_cnt = argc - 2;

    HaarTransform transform;

    HaarFinger src_finger;
    HaarFinger finger;
    HaarFingerDiff diff;

    if (!transform.LoadImage(src_image_path))
    {
        printf("Load image fail: %s\n", src_image_path);
        return 1;
    }

    transform.GetFinger(&src_finger);

    for (int i = 0; i < image_cnt; i++)
    {
        if (!transform.LoadImage(image_path[i]))
        {
            printf("Load image fail: %s\n", image_path[i]);
            return 1;
        }

        transform.GetFinger(&finger);

        if (MatchHaarFinger(src_finger, finger, &diff))
        {
            printf("[match][%s][%.2f][%u][%u][%u][%u]",
                    image_path[i], diff.match_confidence,
                    diff.id_diff, diff.accerator_diff_cnt, 
                    diff.data_diff_cnt, diff.data_valid_cnt);
        }
        else
        {
            printf("[miss ][%s][%.2f][%u][%u][%u][%u]", 
                    image_path[i], diff.match_confidence,
                    diff.id_diff, diff.accerator_diff_cnt, 
                    diff.data_diff_cnt, diff.data_valid_cnt);
        }
    }

    return 0;
}

//gzrd_Lib_CPP_Version_ID--start
#ifndef GZRD_SVN_ATTR
#define GZRD_SVN_ATTR "0"
#endif
static char gzrd_Lib_CPP_Version_ID[] __attribute__((used))="$HeadURL$ $Id$ " GZRD_SVN_ATTR "__file__";
// gzrd_Lib_CPP_Version_ID--end

