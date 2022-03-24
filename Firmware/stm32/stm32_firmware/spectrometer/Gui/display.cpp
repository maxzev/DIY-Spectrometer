#include "display.h"


void display_WriteString(uint16_t x0, uint16_t y0, const char* str, Font font, uint16_t color, uint16_t bgcolor)
{
	ILI9341_WriteString(x0, y0, str, font, color, bgcolor);
}


void display_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color)
{
	ILI9341_DrawFilledRectangle(x0, y0, width, height, color);
}


void display_DrawHollowRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t colour)
{
	ILI9341_DrawHollowRectangle(x0, y0, x0+width, y0+height, colour);
}


void display_DrawHorizontalLine(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color)
{
	//ILI9341_DrawHorizontalLine(x0, y0, width, color);
	ILI9341_DrawFastHLine(x0, y0, width, color);
}


void display_DrawVerticalLine(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color)
{
	//ILI9341_DrawVerticalLine(x0, y0, height, color);
	ILI9341_DrawFastVLine(x0, y0, height, color);
}


void display_FillScreen(uint16_t color)
{
	ILI9341_FillScreen(color);
}


void display_FillScreen(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t colour)
{
	ILI9341_FillRect(x0, y0, width, height, colour);
}

