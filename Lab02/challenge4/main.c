#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Libraries/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Libraries/stb_image.h"

#define STB_DEFINE
#include "Libraries/stb.h"


int main() {
    char input[] = "moodle.jpg";
    char output[] = "moodle.bmp";
    unsigned char *data;
    int w, h, n;

    // TODO 1: Citeste moodle.jpg

    // Programati defensiv cand puteti
    if (!data) {
        printf("Failed!\n");
    }

    // TODO 2: Exporta moodle.bmp in format bmp

    // TODO 3: Elibereaza memoria pentru poza
}
