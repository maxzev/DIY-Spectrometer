#ifndef __ILI9341_DISPLAY_H__
#define __ILI9341_DISPLAY_H__

#include "fonts.h"


#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04


#define ILI9341_SPI_PORT hspi1
extern SPI_HandleTypeDef ILI9341_SPI_PORT;

#define ILI9341_RES_Pin       GPIO_PIN_8
#define ILI9341_RES_GPIO_Port GPIOA
#define ILI9341_CS_Pin        GPIO_PIN_6
#define ILI9341_CS_GPIO_Port  GPIOB
#define ILI9341_DC_Pin        GPIO_PIN_10
#define ILI9341_DC_GPIO_Port  GPIOA


// rotate the screen to the right
#define ILI9341_WIDTH  320
#define ILI9341_HEIGHT 240
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR)


// Color definitions
#define BLACK       0x0000
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF



void ILI9341_Unselect(); // unselect CS of the display

void ILI9341_Init();
void ILI9341_WriteString(uint16_t x0, uint16_t y0, const char* str, Font font, uint16_t color, uint16_t bgcolor);
void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color);
void ILI9341_DrawHorizontalLine(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color);
void ILI9341_DrawVerticalLine(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color);
void ILI9341_FillScreen(uint16_t color);


#endif // __ILI9341_DISPLAY_H__
