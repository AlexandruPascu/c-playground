/* Implement the pinned third-party codecs once, separate from project warnings. */
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_MAX_DIMENSIONS 8192
#include "../third_party/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"
