#include "alpha_blending.h"

int main()
{
    auto back_file  = BMPFile("back.bmp");
    auto cat_file = BMPFile("picture.bmp");

    back_file.compose_alpha(cat_file, 635, 200);
    back_file.save_to_file("composed.bmp");

    return 0;
}