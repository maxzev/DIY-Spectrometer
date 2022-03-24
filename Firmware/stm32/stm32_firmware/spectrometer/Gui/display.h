#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "../ILI9341_display/ili9341_display.h"


void display_WriteString(uint16_t x0, uint16_t y0, const char* str, Font font, uint16_t color, uint16_t bgcolor);
void display_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color);
void display_DrawHollowRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t colour);
void display_DrawHorizontalLine(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
void display_DrawVerticalLine(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color);
void display_FillScreen(uint16_t color);
void display_FillScreen(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t colour);


#endif /* DISPLAY_H_ */
