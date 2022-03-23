
#include "main.h"
#include <math.h>
#include "spectrometer.h"
#include "ili9341_display.h"


#define CHANNELS 288   // from the C12880MA spec
uint16_t data[CHANNELS];
const int DELAY = 250;  // integration time adjustment

// From the spec: the range is 340 -850 nm, distributed between 288 channels.
// There must be something like this: (850 - 340)/288, approximately 1,77 nm per a channel, +/- 12 nm of spectral resolution.
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
	ILI9341_FillScreen(YELLOW);
	delayMicroseconds(1000);

	readSensor();
	ILI9341_FillScreen(DARKGREY);
	HAL_Delay(1000);
	drawChart();
	  //printData();

	HAL_Delay(5000);  //5 sec
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
        delayMicroseconds(DELAY);
        HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
        delayMicroseconds(DELAY);
    }
}


// This function reads spectrometer data from pin VIDEO. See the Timing Chart in spec for details.
static void readSensor()
{
	// Start clock cycle
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
    delayMicroseconds(DELAY);
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
    delayMicroseconds(DELAY);
    HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_SET); //set pin START that starts an integration period (tpi(ST)), see spec
    delayMicroseconds(DELAY); //delay for the SPEC_CLK!

    //Sample for some period of time (depends on integration time you need)
	clockPulseNtimes(15);
	HAL_GPIO_WritePin(SPEC_START_GPIO_Port, SPEC_START_Pin, GPIO_PIN_RESET);
	  //Sample for a period of time. See timing chart tpi(ST).
	clockPulseNtimes(85);

	  //One more clock pulse before the actual read
	clockPulseNtimes(1);

	  //Read channels
	for(int i = 0; i < CHANNELS; ++i)
	{
		HAL_ADC_Start(&SPEC_VIDEO_ADC);
		HAL_ADC_PollForConversion(&SPEC_VIDEO_ADC, HAL_MAX_DELAY);
		data[i] = HAL_ADC_GetValue(&SPEC_VIDEO_ADC);

		clockPulseNtimes(1);
	 }

	 clockPulseNtimes(7);

	 HAL_GPIO_WritePin(SPEC_CLK_GPIO_Port, SPEC_CLK_Pin, GPIO_PIN_SET);
	 delayMicroseconds(DELAY);
}


static void printData()
{
//  for (int i = 0; i < CHANNELS; ++i)
//  {
//    for(int j=0; j< 16; ++j)  //16 values in one row
//    {
//      Serial.print(data[i]);
//      Serial.print(',');
//    }
//    Serial.print("\n");
//  }
//  Serial.print("\n\n\n");
}


static void drawChart()
{
    const uint16_t x0 = 10;
    const uint16_t y0 = 220;

    ILI9341_DrawHorizontalLine(x0, y0, CHANNELS, BLACK);

    //X axis labels (ad hoc approximation!)
    const uint16_t yLabes = y0+7;
    ILI9341_WriteString(x0,     yLabes, "34", Font_7x10, WHITE, DARKGREY); //~340 nm, Violet
    ILI9341_WriteString(x0+45,  yLabes, "45", Font_7x10, WHITE, DARKGREY); //~450 nm, Blue
    ILI9341_WriteString(x0+91,  yLabes, "53", Font_7x10, WHITE, DARKGREY); //~532 nm, Green laser 532nm
    ILI9341_WriteString(x0+125, yLabes, "60", Font_7x10, WHITE, DARKGREY); //~600 nm, Yellow
    ILI9341_WriteString(x0+145, yLabes, "65", Font_7x10, WHITE, DARKGREY); //~650 nm, Oragne
    ILI9341_WriteString(x0+180, yLabes, "70", Font_7x10, WHITE, DARKGREY); //~700 nm, Red
    ILI9341_WriteString(x0+231, yLabes, "75", Font_7x10, WHITE, DARKGREY); //~750 nm, end of visible red
    ILI9341_WriteString(x0+280, yLabes, "x10", Font_7x10, WHITE, DARKGREY); //x10 label


    const float startNm = 340;      // lower bound of CA12880ma's range (see spec)
    // To scale the graph properly there should be integration time adjustment and dark scan subtraction.
    // Currently, the scaleFactor = 20 is used for demonstration purpose.
    const uint16_t scaleFactor = 20; // to fit the line's height inside TFT screen (currently 240x320)

    for (int i = 0; i <CHANNELS; ++i)
    {
      float nm = startNm + i*stepNm;
      RGBstruct color = wavelength2rgb(nm);

      uint16_t red565   = mapVal(color.red,   0, 255, 0, 31);
      uint16_t green565 = mapVal(color.green, 0, 255, 0, 63);
      uint16_t blue565  = mapVal(color.blue,  0, 255, 0, 31);
      uint16_t rgb565 = (red565 << 11)|(green565 << 5)|blue565; //RGB 5+6+5 bits

      uint16_t height = data[i] / scaleFactor;
      uint16_t y1 = y0 - height;

      ILI9341_DrawVerticalLine(x0+i, y1, height, rgb565);
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
