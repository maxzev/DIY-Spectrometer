#include <math.h>
#include <stdio.h>  //for printf
#include <limits.h> // for CHAR_BIT
#include <cstring>
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "main.h"
#include "spectrometer.h"
#include "../Gen/settings.h"
#include "../Gui/display.h"




#define CHANNELS 288   // from the C12880MA spec
uint16_t data[CHANNELS];
uint16_t dark[CHANNELS];
float stepNm =  1.9; // ad hoc


typedef struct
{
   uint8_t red;
   uint8_t green;
   uint8_t blue;
}RGBstruct;


//static funcs declaration
static void clockPulseNtimes(const int nTimes);
static void readSensor();
static void printData();
static void drawChart();
static RGBstruct wavelength2rgb(const float wavelength);
static long mapVal(long x, long in_min, long in_max, long out_min, long out_max);


void runSpectrometer()
{
	readSensor();
	drawChart();

	if(LogToConsole)
	{
	  printData();
	}

	HAL_Delay(2000);
}


void spectrometerInit()
{
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_RESET);
}


static void clockPulseNtimes(const int nTimes)
{
    for(int i=0; i<nTimes; ++i)
    {
        HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
        delayMicroseconds(SpecDelay);
        HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
        delayMicroseconds(SpecDelay);
    }
}


// This function reads spectrometer pin VIDEO. See the Timing Chart in spec for details.
static void readSensor()
{
	// Start clock cycle
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
    delayMicroseconds(SpecDelay);
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
    delayMicroseconds(SpecDelay);
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_SET); //set pin START that starts an integration period (tpi(ST)), see spec
    delayMicroseconds(SpecDelay); //delay for the SPEC_CLK!

    //Sample for some period of time (depends on integration time you need)
	clockPulseNtimes(15);
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_RESET);
	  //Sample for a period of time. See timing chart tpi(ST).
	clockPulseNtimes(85);

	  //One more clock pulse before the actual read
	clockPulseNtimes(1);

	//Read SPEC channels
	for(int i = 0; i < CHANNELS; ++i)
	{
		//fetch only first channel of ADC and start again
		HAL_ADC_Start(&SPEC_VIDEO_ADC);
		HAL_ADC_PollForConversion(&SPEC_VIDEO_ADC, HAL_MAX_DELAY);
		data[i] = HAL_ADC_GetValue(&SPEC_VIDEO_ADC);

		clockPulseNtimes(1);
	 }

	 clockPulseNtimes(7);

	 HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
	 delayMicroseconds(SpecDelay);
}


static void printData()
{
	// Packet format: first number - sensitivity, then 288 channels
	const int maxStrSize =  + (sizeof(SpecDelay)*CHAR_BIT)         // for sensitivity
							+ 5*CHANNELS
							+ 288 +1;                              // 288 for "," between and +1 for '\0'
	char str[maxStrSize];
	int strPos = 0;
	strPos += sprintf(str+strPos, "%d", SpecDelay);
	if(strPos <0)
		return; //error!

	str[strPos] = ',';
	++strPos;

	for (int i = 0; i < CHANNELS; ++i)
	{
		strPos += sprintf(str+strPos, "%d", data[i]);
		if(strPos < 0)
			return; //error!
		str[strPos] = ',';
		++strPos;
	}

	--strPos; // remove last ','

	str[strPos] = '\0';
	uint16_t len = strlen(str);

	CDC_Transmit_FS((uint8_t*)str, len);
}


//Returns wavelength for every channel of the sensor.
//See https://groupgets.com/manufacturers/hamamatsu-photonics/products/c12880ma-micro-spectrometer
// for calibration formula and calibration data according to your sensor serial number.
static float channel2wavelength(int CN)
{
	const float a0 = 310.9075849;
	const float b1 = 2.714280624;
	const float b2 = -0.001329954279;
	const float b3 = -6.961112807E-06;
	const float b4 = 8.397003579E-09;
	const float b5 = 5.027463790E-12;

	float r = a0 + b1*CN + b2*pow(CN, 2) + b3*pow(CN, 3) + b4*pow(CN, 4) + b5*pow(CN,5);

	return r;
}



static void drawChart()
{
    const uint16_t x0 = 10;
    const uint16_t y0 = 220;

    display_DrawHorizontalLine(x0, y0, CHANNELS, BLACK);

    //X axis labels. See https://groupgets.com/manufacturers/hamamatsu-photonics/products/c12880ma-micro-spectrometer
    // for calibration formula and calibration data according to your sensor serial number.
    const uint16_t yLabes = y0+7;
    display_WriteString(x0,     yLabes, "32", Font_7x10, WHITE, DARKGREY); //~313 nm, Violet
    display_WriteString(x0+45,  yLabes, "43", Font_7x10, WHITE, DARKGREY); //~429 nm, Blue
    display_WriteString(x0+87,  yLabes, "53", Font_7x10, WHITE, DARKGREY); //~532 nm, Green laser 532nm
    display_WriteString(x0+117, yLabes, "60", Font_7x10, WHITE, DARKGREY); //~600 nm, Yellow
    display_WriteString(x0+140, yLabes, "65", Font_7x10, WHITE, DARKGREY); //~650 nm, Orange
    display_WriteString(x0+166, yLabes, "70", Font_7x10, WHITE, DARKGREY); //~700 nm, Red
    display_WriteString(x0+200, yLabes, "76", Font_7x10, WHITE, DARKGREY); //~760 nm, end of visible red
    display_WriteString(x0+280, yLabes, "x10", Font_7x10, WHITE, DARKGREY); //x10 label


    // Note: There should be the integration time adjustment(sensitivity) and dark scan subtraction
    //       to scale the graph on the display properly.
    //       Currently, the scaleFactor = 20 is used
    //       to fit the line's height inside TFT screen (currently 240x320).
    const uint16_t scaleFactor = 20; //

    // NOTE: dark scan values vary from channel to channel and depend on sensitivity and sensor type.
    //       The value around 550-600 is dark scan for my sensor with the smallest sensitivity.
    //       Can be used safely for any other sensitivity to fit the chart inside the display.
    const uint16_t darkScan = 550;

    for(int i=0; i<=CHANNELS; ++i)
    {
    	uint16_t y1 = y0 - dark[i];
    	display_DrawVerticalLine(x0+i, y1, dark[i], DARKGREY);
    }

    for (int i = 1; i <=CHANNELS; ++i)
    {
      float nm = channel2wavelength(i);
      RGBstruct color = wavelength2rgb(nm);

      uint16_t red565   = mapVal(color.red,   0, 255, 0, 31);
      uint16_t green565 = mapVal(color.green, 0, 255, 0, 63);
      uint16_t blue565  = mapVal(color.blue,  0, 255, 0, 31);
      uint16_t rgb565 = (red565 << 11)|(green565 << 5)|blue565; //RGB 5+6+5 bits

      uint16_t height = (data[i-1] - darkScan) / scaleFactor;
      uint16_t y1 = y0 - height;

      display_DrawVerticalLine(x0+i, y1, height, rgb565);

      dark[i] = height;
    }//for
}





// Wavelength to RGB approximation
static RGBstruct wavelength2rgb(const float wavelength) // wavelength 340 to 850 nm
{
   // Credits: Dr. Dan Bruton http://danbruton.com/
   float red   = 0.0;
   float green = 0.0;
   float blue  = 0.0;

   if ((wavelength >= 350.0) && (wavelength < 440.0))
   {
      red   = -(wavelength - 440.0) / (440.0 - 350.0);
      green = 0.0;
      blue  = 1.0;
   }
   else if ((wavelength >= 440.0) && (wavelength < 490.0))
   {
      red   = 0.0;
      green = (wavelength - 440.0) / (490.0 - 440.0);
      blue  = 1.0;
   }
   else if ((wavelength >= 490.0) && (wavelength < 510.0))
   {
      red   = 0.0;
      green = 1.0;
      blue  = -(wavelength - 510.0) / (510.0 - 490.0);
   }
   else if ((wavelength >= 510.0) && (wavelength < 580.0))
   {
      red   = (wavelength - 510.0) / (580.0 - 510.0);
      green = 1.0;
      blue  = 0.0;
   }
   else if ((wavelength >= 580.0) && (wavelength < 645.0))
   {
      red   = 1.0;
      green = -(wavelength - 645.0) / (645.0 - 580.0);
      blue  = 0.0;
   }
   else if ((wavelength >= 645.0) && (wavelength <= 850.0))
   {
      red   = 1.0;
      green = 0.0;
      blue  = 0.0;
   }
   else
   {
      red   = 0.0;
      green = 0.0;
      blue  = 0.0;
   }

   float factor = 0.0;

   // Let the intensity fall off near the vision limit.
   if ((wavelength >= 701.0) && (wavelength < 850.0))
      factor = 0.3 + 0.7 * (850.0 - wavelength) / (850.0 - 701.0);

   else if ((wavelength >= 420.0) && (wavelength < 701.0))
      factor = 1.0;

   else if ((wavelength >= 350.0) && (wavelength < 420.0))
      factor = 0.3 + 0.7 * (wavelength - 350.0) / (420.0 - 350.0);

   RGBstruct result;

   const float gamma = 0.8;
   result.red   = (uint8_t)((0.0 == red)   ? red   : floorf((255.0 * pow(red   * factor, gamma)) + 0.5));
   result.green = (uint8_t)((0.0 == green) ? green : floorf((255.0 * pow(green * factor, gamma)) + 0.5));
   result.blue  = (uint8_t)((0.0 == blue)  ? blue  : floorf((255.0 * pow(blue  * factor, gamma)) + 0.5));

   return result;
}


static long mapVal(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
