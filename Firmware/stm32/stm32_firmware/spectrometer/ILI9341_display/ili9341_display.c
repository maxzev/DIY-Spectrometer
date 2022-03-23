#include "stm32f4xx_hal.h"
#include "ili9341_display.h"


static void ILI9341_Select()
{
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_RESET);
}


void ILI9341_Unselect()
{
    HAL_GPIO_WritePin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin, GPIO_PIN_SET);
}


static void ILI9341_Reset()
{
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ILI9341_RES_GPIO_Port, ILI9341_RES_Pin, GPIO_PIN_SET);
}


static void ILI9341_WriteCommand(uint8_t cmd)
{
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ILI9341_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}


static void ILI9341_WriteData(uint8_t* buff, size_t buff_size)
{
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    // split data in small chunks because HAL can't send more then 64K at once
    while(buff_size > 0)
    {
        uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
        HAL_SPI_Transmit(&ILI9341_SPI_PORT, buff, chunk_size, HAL_MAX_DELAY);
        buff += chunk_size;
        buff_size -= chunk_size;
    }
}


static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        uint8_t data[] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        uint8_t data[] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1 & 0xFF };
        ILI9341_WriteData(data, sizeof(data));
    }

    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}


void ILI9341_Init()
{
	ILI9341_Unselect();
    ILI9341_Select();
    ILI9341_Reset();

    // command list is based on https://github.com/martnak/STM32-ILI9341

    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    HAL_Delay(1000);
        
    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint8_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint8_t data[] = { 0x00, 0xC1, 0x30 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint8_t data[] = { 0x85, 0x00, 0x78 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0xEA);
    {
        uint8_t data[] = { 0x00, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint8_t data[] = { 0x64, 0x03, 0x12, 0x81 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint8_t data[] = { 0x20 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint8_t data[] = { 0x23 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint8_t data[] = { 0x10 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint8_t data[] = { 0x3E, 0x28 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint8_t data[] = { 0x86 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = { 0x48 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint8_t data[] = { 0x55 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint8_t data[] = { 0x00, 0x18 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint8_t data[] = { 0x08, 0x82, 0x27 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint8_t data[] = { 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint8_t data[] = { 0x01 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint8_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                           0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ILI9341_WriteData(data, sizeof(data));
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint8_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                           0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ILI9341_WriteData(data, sizeof(data));
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint8_t data[] = { ILI9341_ROTATION };
        ILI9341_WriteData(data, sizeof(data));
    }

    ILI9341_Unselect();
}


static void ILI9341_WriteChar(uint16_t x0, uint16_t y0, char ch, Font font, uint16_t color, uint16_t bgcolor)
{
    ILI9341_SetAddressWindow(x0, y0, x0 +font.width -1, y0 +font.height -1);

    for(uint32_t i=0; i < font.height; ++i)
    {
    	uint32_t b = font.data[(ch -32)*font.height+i];

        for( uint32_t j=0; j < font.width; ++j)
        {
            if((b << j) & 0x8000)
            {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
            else
            {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ILI9341_WriteData(data, sizeof(data));
            }
        }//for
    }//for
}


void ILI9341_WriteString(uint16_t x0, uint16_t y0, const char* str, Font font, uint16_t color, uint16_t bgcolor)
{
    ILI9341_Select();

    while(*str)
    {
        if(x0 + font.width >= ILI9341_WIDTH)
        {
            x0 = 0;
            y0 += font.height;

            if(y0 + font.height >= ILI9341_HEIGHT)
                break;

            if(*str == ' ') // skip spaces in the beginning of the new line
            {
                ++str;
                continue;
            }
        }

        ILI9341_WriteChar(x0, y0, *str, font, color, bgcolor);
        x0 += font.width;
        ++str;
    }

    ILI9341_Unselect();
}


void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color)
{
    if((x0 >= ILI9341_WIDTH) || (y0 >= ILI9341_HEIGHT))
    	return;
    if((x0 + width -1) >= ILI9341_WIDTH)
    	width = ILI9341_WIDTH -x0;
    if((y0 + height -1) >= ILI9341_HEIGHT)
    	height = ILI9341_HEIGHT -y0;

    ILI9341_Select();
    ILI9341_SetAddressWindow(x0, y0, x0+width-1, y0+height-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

    for(y0 = height; y0 > 0; --y0)
    {
        for(x0 = width; x0 > 0; --x0)
            HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
    }

    ILI9341_Unselect();
}


void ILI9341_DrawVerticalLine(uint16_t x0, uint16_t y0, uint16_t height, uint16_t color)
{
	if((x0 >= ILI9341_WIDTH) || (y0 >= ILI9341_HEIGHT))
	    	return;
	if((y0 + height -1) >= ILI9341_HEIGHT)
	    	height = ILI9341_HEIGHT -y0;

	ILI9341_Select();
	ILI9341_SetAddressWindow(x0, y0, x0, y0+height-1);

	uint8_t data[] = { color >> 8, color & 0xFF };
	HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

	for(int i=0; i< height; ++i)
	{
		HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
	}

	ILI9341_Unselect();
}


void ILI9341_DrawHorizontalLine(uint16_t x0, uint16_t y0, uint16_t width, uint16_t color)
{
	if((x0 >= ILI9341_WIDTH) || (y0 >= ILI9341_HEIGHT))
	    return;
	if((x0 + width -1) >= ILI9341_WIDTH)
	   width = ILI9341_WIDTH -x0;

	ILI9341_Select();
	ILI9341_SetAddressWindow(x0, y0, x0+width-1, y0);

	uint8_t data[] = { color >> 8, color & 0xFF };
	HAL_GPIO_WritePin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin, GPIO_PIN_SET);

	for(int i=0; i< width; ++i)
	{
		HAL_SPI_Transmit(&ILI9341_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
	}

	ILI9341_Unselect();
}


void ILI9341_FillScreen(uint16_t color)
{
	ILI9341_DrawFilledRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}





