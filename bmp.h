#include <stdint.h>
#include <string.h>

typedef struct tagBMPFILEHEADER{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserver2;
    uint32_t image_offset;
}BMPFILEHEADER;

typedef struct tagBMPINFOHEADER{
    uint32_t header_size;
    uint32_t wigth;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels;
    uint32_t y_pixels;
    uint32_t color_used;
    uint32_t color_important;

}BMPINFOHEADER;

struct BMP_Image{
    BMPINFOHEADER *header;
    uint8_t *palette;
    uint8_t *rgb;
}IMAGE;