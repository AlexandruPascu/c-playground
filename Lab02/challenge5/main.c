#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Libraries/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"

#define STB_DEFINE
#include "Libraries/stb.h"


int main() {
    char input[] = "r3kt.jpg";
    char output_average[] = "r3kt_average.png";
    char output_luminosity[] = "r3kt_luminosity.png";
    unsigned char *data_average;
    unsigned char *data_luminosity;
    int w, h, n;

    data_average = stbi_load(input, &w, &h, &n, 4);
    data_luminosity = stbi_load(input, &w, &h, &n, 4);

    for (int i = 0; i <  w * h * 4; ++i) {
        // TODO 1: Calculeaza o valoare average si asigneaz-o componentelor
        // RGB
    }

    stbi_write_png(output_average, w, h, 4, data_average, 4 * w);
    stbi_image_free(data_average);

    for (int i = 0; i <  w * h * 4; ++i) {
        // TODO 2: Aplica formula pentru a calcula valoarea average si aplic-o
        // componentelor RGB
    }

    stbi_write_png(output_luminosity, w, h, 4, data_luminosity, 4 * w);
    stbi_image_free(data_luminosity);


}
