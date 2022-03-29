#include "mainmenu.h"
#include "display.h"
#include "../Gen/keypad.h"

bool MainMenuActive = false;
//const int MainMenuItemNum = 3; // currently: Sensitivity, Presets, Dark Scan.
int MainMenuPos = 0;

void mainMenu()
{
	//TODO: draw menu
	// start to listen to buttons pressed
	//ILI9341_DrawHollowRectangle(80, 50, 250, 150, CYAN);
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


	//select
	display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, RED);

	while(1)
	{
		int pressedButton = keypad_checkButtons();

		if(0 == pressedButton) // Ok
		{

		}
		else if(1 == pressedButton) //Up
		{

		}
		else if(2 == pressedButton) //Down
		{

		}

		if(!MainMenuActive)
			break;

	}//while

}
