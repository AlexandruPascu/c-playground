#include "apps.h"
#include "image_ops.h"
#include "pin.h"
#include "../common/input.h"
#include "../third_party/stb/stb_image.h"
#include "../third_party/stb/stb_image_write.h"
#include <string.h>
#ifndef PLAYGROUND_SOURCE_DIR
#define PLAYGROUND_SOURCE_DIR "."
#endif
#ifndef PLAYGROUND_OUTPUT_DIR
#define PLAYGROUND_OUTPUT_DIR "."
#endif
static unsigned char *load(const char *path, int *width, int *height) {
    int channels;
    unsigned char *data;
    if (!stbi_info(path, width, height, &channels) || *width < 1 || *height < 1 ||
        *width > 8192 || *height > 8192 || (size_t)*width * (size_t)*height > 16000000) {
        fprintf(stderr, "Cannot read image metadata, or image exceeds 8192 per side / 16000000 pixels: %s\n", path);
        return NULL;
    }
    data = stbi_load(path, width, height, &channels, 4);
    if (!data) fprintf(stderr, "Cannot decode image: %s\n", path);
    return data;
}
static int save_png(const char *path, int width, int height, const unsigned char *data) {
    if (!stbi_write_png(path, width, height, 4, data, width * 4)) { fprintf(stderr, "Cannot write image: %s\n", path); return 0; }
    printf("Wrote %s (%dx%d)\n", path, width, height);
    return 1;
}
int pg_convert(int argc, char **argv) {
    int width, height, success;
    unsigned char *data;
    const char *input = argc == 3 ? argv[1] : PLAYGROUND_SOURCE_DIR "/Lab02/challenge4/moodle.jpg";
    const char *output = argc == 3 ? argv[2] : PLAYGROUND_OUTPUT_DIR "/moodle.bmp";
    if (argc == 2 && !strcmp(argv[1], "--help")) { puts("Usage: image_convert [input.jpg output.bmp]\nLoads JPEG/PNG/BMP and writes a BMP image. No arguments uses the workshop sample."); return 0; }
    if (argc != 1 && argc != 3) { fputs("Use image_convert [input output.bmp].\n", stderr); return 1; }
    if (!strcmp(input, output)) { fputs("Input and output must differ.\n", stderr); return 1; }
    data = load(input, &width, &height);
    if (!data) return 1;
    success = stbi_write_bmp(output, width, height, 4, data);
    stbi_image_free(data);
    if (!success) { fprintf(stderr, "Cannot write image: %s\n", output); return 1; }
    printf("Wrote %s (%dx%d)\n", output, width, height);
    return 0;
}
int pg_grayscale(int argc, char **argv) {
    int width, height, success = 0;
    unsigned char *original, *average;
    size_t bytes;
    const char *input = argc == 4 ? argv[1] : PLAYGROUND_SOURCE_DIR "/Lab02/challenge5/r3kt.jpg";
    const char *output1 = argc == 4 ? argv[2] : PLAYGROUND_OUTPUT_DIR "/grayscale_average.png";
    const char *output2 = argc == 4 ? argv[3] : PLAYGROUND_OUTPUT_DIR "/grayscale_weighted.png";
    if (argc == 2 && !strcmp(argv[1], "--help")) { puts("Usage: image_grayscale [input average.png weighted.png]\nWrites mean-RGB and weighted (0.2126 R + 0.7152 G + 0.0722 B) grayscale. Alpha is preserved."); return 0; }
    if (argc != 1 && argc != 4) { fputs("Use image_grayscale [input average.png weighted.png].\n", stderr); return 1; }
    if (!strcmp(input, output1) || !strcmp(input, output2) || !strcmp(output1, output2)) { fputs("Image paths must differ.\n", stderr); return 1; }
    original = load(input, &width, &height);
    if (!original) return 1;
    bytes = (size_t)width * (size_t)height * 4;
    average = malloc(bytes);
    if (average) {
        memcpy(average, original, bytes);
        pg_gray(average, bytes / 4, 0);
        pg_gray(original, bytes / 4, 1);
        success = save_png(output1, width, height, average) && save_png(output2, width, height, original);
        free(average);
    } else fputs("Could not allocate grayscale image.\n", stderr);
    stbi_image_free(original);
    return !success;
}
int pg_decode(int argc, char **argv) {
    int width, height, pin, success;
    unsigned int passes;
    char recovered[5];
    unsigned char *data;
    const char *input = argc >= 3 ? argv[1] : PLAYGROUND_SOURCE_DIR "/Lab02/challenge6/input.png";
    const char *output = argc >= 3 ? argv[2] : PLAYGROUND_OUTPUT_DIR "/decoded.png";
    if (argc == 2 && !strcmp(argv[1], "--help")) { puts("Usage: image_decode [input.png output.png [PIN]]\nReverses the workshop's rolling XOR, PIN modulo the greatest prime below 200 times.\nDefault PIN is recovered from Razvan's supplied workshop hash. This is an obfuscation exercise."); return 0; }
    if (argc != 1 && argc != 3 && argc != 4) { fputs("Use image_decode [input.png output.png [PIN]].\n", stderr); return 1; }
    if (!strcmp(input, output)) { fputs("Input and output must differ.\n", stderr); return 1; }
    if (!pg_find_pin((uint32_t)(int32_t)-503189101, recovered) || !pg_parse_int(recovered, &pin)) return 1;
    if (argc == 4 && (!pg_parse_int(argv[3], &pin) || pin < 0 || pin > 9999)) { fputs("PIN must be an integer from 0 to 9999.\n", stderr); return 1; }
    passes = (unsigned int)(pin % pg_prime_below(200));
    data = load(input, &width, &height);
    if (!data) return 1;
    pg_xor_decode(data, (size_t)width * (size_t)height * 4, passes);
    success = save_png(output, width, height, data);
    stbi_image_free(data);
    if (success) printf("Reversed %u XOR passes (PIN %04d).\n", passes, pin);
    return !success;
}
