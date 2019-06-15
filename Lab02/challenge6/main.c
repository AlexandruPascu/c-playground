#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Libraries/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"

#define STB_DEFINE
#include "Libraries/stb.h"

// TODO 1: Inlocuieste 10 cu pin-ul lui Razvan din lab-ul trecut
// TODO 2: Gaseste cel mai mare numar prim mai mic decat 200
#define PIN_RAZVAN 10
#define NICE_PRIME 1

int main() {

    char input[] = "input.png";
    char output[] = "output.png";
    unsigned char *data;
    int w, h, n;

    data = stbi_load(input, &w, &h, &n, 4);

    if (!data) {
        printf("Failed!\n");
    }

    // TODO 3: Decripteaza imaginea

    stbi_write_png(output, w, h, 4, data, 4 * w);
    stbi_image_free(data);
}
