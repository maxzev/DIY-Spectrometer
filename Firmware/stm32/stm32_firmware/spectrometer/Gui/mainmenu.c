#include "mainmenu.h"
#include "display.h"
#include "../Gen/keypad.h"

#include "adc.h"


bool MainMenuActive = false;
static const int MainMenuItemNum = 3; // there are 3 menu items per each screen currently
int CurrentMenuPos = 0;

static uint16_t elementWidth  = 240;
static uint16_t elementHeight = 30;
static uint16_t elementColor  = 0xEDEA;


void menuItemUp()
{
	if(0 == CurrentMenuPos)
		return;

	--CurrentMenuPos;

	HAL_Delay(500); //debounce
}


void menuItemDown()
{
	if(CurrentMenuPos < MainMenuItemNum-1)
		++CurrentMenuPos;

	HAL_Delay(500); //debounce
}


void mainMenu()
{
	//todo: drawMainMenu();
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "MENU";
	display_WriteString(130, 30, title, Font_16x26, OLIVE, CYAN);

	uint16_t elementWidth  = 240;
	uint16_t elementHeight = 30;
	uint16_t elementColor = 0xEDEA;

	display_DrawFilledRectangle(40, 70, elementWidth, elementHeight, elementColor);
	const char* sens = "Sensitivity";
	display_WriteString(105, 75, sens, Font_11x18, BLUE, elementColor);

	display_DrawFilledRectangle(40, 120, elementWidth, elementHeight, elementColor);
	const char* pres = "Presets";
	display_WriteString(125, 125, pres, Font_11x18, BLUE, elementColor);

	display_DrawFilledRectangle(40, 170, elementWidth, elementHeight, elementColor);
	const char* darkscan = "Dark Scan elim.";
	display_WriteString(85, 175, darkscan, Font_11x18, BLUE, elementColor);

	//TODO: add PC connection settings button

	//select the first item by default
	display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, RED);
	CurrentMenuPos = 0;


	while(1)
	{
		HAL_ADC_Start(&hadc1);
		//skip first ADC channel's value
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		int pressedButton = HAL_ADC_GetValue(&hadc1);

		//fetch KEYPAD ADC channel's value
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		pressedButton = HAL_ADC_GetValue(&hadc1);

		if(pressedButton < 3500)
		{
			if((pressedButton <= 1500) && (pressedButton > 500)) //Ok button
			{
				drawCurrentMenu();

			}
			else if((pressedButton <= 2500) && (pressedButton > 1500)) // Up button
			{
				unselectCurrentMenu();
				menuItemUp();
				selectCurrentMenu();
			}
			else if((pressedButton <= 3500) && (pressedButton > 2500)) // Down button
			{
				unselectCurrentMenu();
				menuItemDown();
				selectCurrentMenu();
			}
		}

//		if(CurrentMenuPos != 1 && CurrentMenuPos != 2 && CurrentMenuPos != 0)
//		{
//			HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_RESET);
//			HAL_Delay(500);
//			HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);
//			HAL_Delay(500);
//			HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_RESET);
//			HAL_Delay(500);
//			HAL_GPIO_WritePin(ONBOARD_LED_GPIO_Port, ONBOARD_LED_Pin, GPIO_PIN_SET);
//			HAL_Delay(500);
//		}


		if(!MainMenuActive)
			break;

	}//while
}


void drawCurrentMenu()
{
	//todo: refactor me!
	if(0 == CurrentMenuPos)
	{
		sensitivityMenu();
	}
	else if(1 == CurrentMenuPos)
	{
		presetsMenu();
	}
	else if(2 == CurrentMenuPos)
	{
		darkScanMenu();
	}
}


void sensitivityMenu()
{
	//draw menu
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "Sensitivity";
	display_WriteString(70, 30, title, Font_16x26, OLIVE, CYAN);

	//todo: listen to the keypad

}


void presetsMenu()
{
	//draw menu
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "Presets";
	display_WriteString(110, 30, title, Font_16x26, OLIVE, CYAN);

	//todo: listen to the keypad
}


void darkScanMenu()
{
	//draw menu
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "Dark Scan elim.";
	display_WriteString(45, 30, title, Font_16x26, OLIVE, CYAN);

	//todo: listen to the keypad
}


void selectCurrentMenu()
{
	if(0 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, RED);
	}
	else if(1 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 119, elementWidth+1, elementHeight+1, RED);
	}
	else if(2 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 169, elementWidth+1, elementHeight+1, RED);
	}
}


void unselectCurrentMenu()
{
	//todo: refactor me!
	if(0 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, CYAN);
	}
	else if(1 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 119, elementWidth+1, elementHeight+1, CYAN);
	}
	else if(2 == CurrentMenuPos)
	{
		display_DrawHollowRectangle(39, 169, elementWidth+1, elementHeight+1, CYAN);
	}
}
