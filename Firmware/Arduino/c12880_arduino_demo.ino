/*
*
* Single-file demo for Arduino board. 
* Reads values from spectrometer and displays the spectrum on the TFT screen every 5 seconds.
*
* GNU Lesser General Public License v3.0
*
* Copyright by Max Zev max_zev@protonmail.com
* https://github.com/maxzev/DIY_Spectrometer
* 
* Tested with Arduino Uno, Mega with ATmega328 or ATmega2560 processor.  
*
*/

//Implmentation is based on: https://github.com/groupgets/c12880ma/blob/master/arduino_c12880ma_example/arduino_c12880ma_example.ino 

#include <math.h>
#include <TFT_ILI9341.h>  //https://github.com/Bodmer/TFT_ILI9341


// BreakoutV2 board pins to Arduino pins
#define PIN_START       A1
#define PIN_CLK         A2
#define PIN_VIDEO       A3
#define PIN_LED         A4  // breakout board led

const int CHANNELS = 288;     // from the C12880MA spec
uint16_t data[CHANNELS];
const int DELAY = 500;

float stepNm =  1.9; // ad hoc

// TFT pinout, see also  <libraries_dir>/TFT_ILI9341/User_Setup.h
//const uint8_t TFT_RST  = 8;
//const uint8_t TFT_DC   = 9;
//const uint8_t TFT_CS   = 10;
//const uint8_t TFT_MOSI = 11;
//const uint8_t TFT_SCK  = 13;


TFT_ILI9341 tft = TFT_ILI9341();


struct RGBstruct
{
   uint8_t red   = 0;
   uint8_t green = 0;
   uint8_t blue  = 0;
};


void setup()
{
  tft.init();
  tft.setRotation(3); // X axis along the long side of the screen
  
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_CLK, HIGH); 
  digitalWrite(PIN_START, LOW); 

  Serial.begin(115200); 
}


void clockPulseNtimes(const int nTimes)
{
    for(int i=0; i<nTimes; ++i)
    {
        digitalWrite(PIN_CLK, HIGH);
        delayMicroseconds(DELAY);
        digitalWrite(PIN_CLK, LOW);
        delayMicroseconds(DELAY); 
    }
}


// This function reads spectrometer data from PIN_VIDEO. See the Timing Chart in spec for details.
void readSensor()
{
  // Start clock cycle
  digitalWrite(PIN_CLK, LOW);
  delayMicroseconds(DELAY);
  digitalWrite(PIN_CLK, HIGH);
  delayMicroseconds(DELAY);
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_START, HIGH); //set PIN_START that starts an integration period (tpi(ST)), see spec
  delayMicroseconds(DELAY);      //delay for the PIN_CLK!

  //Sample for some period of time (depends on integration time you need)
  clockPulseNtimes(15);  

  digitalWrite(PIN_START, LOW); //Set PIN_START to start reading

  //Sample for a period of time. See timing chart tpi(ST).
  clockPulseNtimes(85);

  //One more clock pulse before the actual read
  clockPulseNtimes(1);

  //Read channels
  for(int i = 0; i < CHANNELS; ++i)
  {
      data[i] = analogRead(PIN_VIDEO);
      
      clockPulseNtimes(1);  
  }

  clockPulseNtimes(7);  

  digitalWrite(PIN_CLK, HIGH);
  delayMicroseconds(DELAY);
}


void printData()
{
  for (int i = 0; i < CHANNELS; ++i)
  {  
    for(int j=0; j< 16; ++j)  //16 values in one row
    {
      Serial.print(data[i]);
      Serial.print(',');
    }
    Serial.print("\n");
  }
  Serial.print("\n\n\n");
}


void loop()
{
  Serial.println("Start...");
  delayMicroseconds(1000);
  
  readSensor();
  tft.fillScreen(TFT_DARKGREY);
  delay(1000);
  drawChart();
  //printData();
  
  delay(5000);  //5 sec
}


void drawChart()
{
    const int x0 = 10;
    const int y0 = 220;

    tft.drawFastHLine(x0, y0, 288, TFT_BLACK);

    //X axis labels (ad hoc approximation!)
    String xLabels[]={"32", "43", "53", "60", "65", "70", "76", "x10"};
    const int yLabes = y0+7;
    tft.setCursor(x0, yLabes);    //~320 nm, Violet 
    tft.print(xLabels[0]);
    tft.setCursor(x0+45, yLabes); //~429 nm, Blue 
    tft.print(xLabels[1]);
    tft.setCursor(x0+87, yLabes); //~532 nm, Green laser 532nm
    tft.print(xLabels[2]);
    tft.setCursor(x0+117, yLabes);//~600 nm, Yellow 
    tft.print(xLabels[3]);
    tft.setCursor(x0+140, yLabes);//~650 nm, Orange 
    tft.print(xLabels[4]);
    tft.setCursor(x0+166, yLabes);//~700 nm, Red 
    tft.print(xLabels[5]);
    tft.setCursor(x0+200, yLabes);//~750 nm, end of visible red 
    tft.print(xLabels[6]);
    tft.setCursor(x0+280, yLabes);//x10 label
    tft.print(xLabels[7]);
  
    
    const float startNm = 340;      // lower bound of CA12880ma's range (see spec)
    const uint16_t scaleFactor = 5; // to fit the line's height inside TFT screen (currently 240x320)

    for (int i = 2; i <288; ++i) 
    {
      float nm = startNm + i*stepNm;
      RGBstruct color = wavelength2rgb(nm);

      uint16_t red565   = map(color.red,   0, 255, 0, 31);
      uint16_t green565 = map(color.green, 0, 255, 0, 63);
      uint16_t blue565  = map(color.blue,  0, 255, 0, 31);
      uint16_t rgb565 = (red565 << 11)|(green565 << 5)|blue565; //RGB 5+6+5 bits
      
      uint16_t height = data[i] / scaleFactor;
      uint16_t topCoordinate = y0 - height;
      tft.drawLine(x0+i, y0, x0+i, topCoordinate, rgb565);
    }
}


// Wavelength to RGB approximation
RGBstruct wavelength2rgb(const float wavelength) // wavelength 340 to 850 nm
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
   result.red   = static_cast<uint8_t>((0.0 == red)   ? red   : floorf((255.0 * pow(red   * factor, gamma)) + 0.5));
   result.green = static_cast<uint8_t>((0.0 == green) ? green : floorf((255.0 * pow(green * factor, gamma)) + 0.5));
   result.blue  = static_cast<uint8_t>((0.0 == blue)  ? blue  : floorf((255.0 * pow(blue  * factor, gamma)) + 0.5));

   return result;
}
