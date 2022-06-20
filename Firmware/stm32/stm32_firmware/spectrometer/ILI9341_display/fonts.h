#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

// Credits to https://github.com/afiskon/stm32-ili9341

typedef struct
{
    const uint8_t width;
    const uint8_t height;
    const uint16_t* data;
} Font;

extern Font Font_7x10;
extern Font Font_11x18;
extern Font Font_16x26;

#endif // __FONTS_H__
