#include "mainmenu.h"
#include "display.h"
#include "../Gen/keypad.h"

#include "adc.h"


bool MainMenuActive = false;
bool MenuOk   = false;
bool MenuUp   = false;
bool MenuDown = false;

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
}


void menuItemDown()
{
	if(CurrentMenuPos < MainMenuItemNum-1)
		++CurrentMenuPos;
}


void menuItemReset()
{
	CurrentMenuPos = 0;
}


void drawMainMenu()
{
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

	//select the first item by default
	menuItemReset();
	display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, RED);
}


void mainMenu()
{
	drawMainMenu();

	while(1)
	{
		if(MenuOk)
		{
			MenuOk = false;
			drawCurrentMenu();
		}
		else if(MenuUp)
		{
			MenuUp = false;
			unselectCurrentMenu();
			menuItemUp();
			selectCurrentMenu();
		}
		else if(MenuDown)
		{
			MenuDown = false;
			unselectCurrentMenu();
			menuItemDown();
			selectCurrentMenu();
		}

		if(!MainMenuActive)
		{
			const char* title = "Exiting ...";
			display_WriteString(90, 100, title, Font_16x26, RED, CYAN);
			break;
		}
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
