#include <Magick++.h> 
#include <iostream>

using namespace std; 
using namespace Magick; 

int main(int argc, char *argv[])
{
    const char *image_path = argv[1];

    Image image;

    try { 
        image.read("girl.gif");
        image.resize(Geometry(128, 128));
        image.quantizeColorSpace(GRAYColorspace); 
        image.quantizeColors(256); 
        image.quantize();
        image.write(string(image_path) + ".256.jpg");
    } 
    catch (Exception &error) 
    { 
        cout << "Caught exception: " << error.what() << endl; 
        return 1; 
    }

    return 0;
}
