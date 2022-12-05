#include <iostream>
#include <fstream>
#include <string>
#include <optional>
#include <cstdio>

#include "Scene/Scene.hpp"
#include "Scene/Ray.hpp"
#include "Scene/Sphere.hpp"

#pragma warning(disable:4996)

void save_imageP6(int Width, int Height, const char* fname, unsigned char* pixels) {
    FILE* fp;
    const int maxVal = 255;

    //Reflect the image vertically
    uint32_t rowLength = Width * 3;
    uint8_t* temp = new uint8_t[rowLength];

    for (uint64_t i = 0; i < Height / 2; ++i)
    {
        uint8_t* row1 = pixels + i * rowLength;
        uint8_t* row2 = pixels + (Height - i - 1) * rowLength;

        memcpy(temp, row1, rowLength);
        memcpy(row1, row2, rowLength);
        memcpy(row2, temp, rowLength);
    }

    delete[] temp;

    printf("Saving image %s: %d x %d\n", fname, Width, Height);
    fp = fopen(fname, "wb");
    if (!fp) {
        printf("Unable to open file '%s'\n", fname);
        return;
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", Width, Height);
    fprintf(fp, "%d\n", maxVal);

    for (int j = 0; j < Height; j++) {
        fwrite(&pixels[j * Width * 3], 3, Width, fp);
    }

    fclose(fp);
}

int main(int argc, char** argv)
{
    std::string filename(argv[1]);
    Scene scene(filename);

    Image image = scene.Render();

    save_imageP6(image.x, image.y, image.name.c_str(), reinterpret_cast<unsigned char*>(image.pixels));

    return 0;
}
