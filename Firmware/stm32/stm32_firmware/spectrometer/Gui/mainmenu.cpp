#include "mainmenu.h"
#include "display.h"
#include "sensitivity_menu.h"


static const int MainMenuMaxNum = 3; // there are 3 menu items per each screen currently
static uint16_t elementWidth  = 240;
static uint16_t elementHeight = 30;
static uint16_t elementColor  = 0xEDEA;


static void selectCurrentMenu()
{
	if(CurrentMenuPos >= MainMenuMaxNum)
		return; // todo: handle an error!

	uint16_t arr [MainMenuMaxNum] = {69, 119, 169};
	display_DrawHollowRectangle(39, arr[CurrentMenuPos], elementWidth+1, elementHeight+1, RED);
}


static void unselectCurrentMenu()
{
	if(CurrentMenuPos >= MainMenuMaxNum)
		return; // todo: handle an error!

	uint16_t arr [MainMenuMaxNum] = {69, 119, 169};
	display_DrawHollowRectangle(39, arr[CurrentMenuPos], elementWidth+1, elementHeight+1, CYAN);
}


void drawMainMenu()
{
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "MENU";
	display_WriteString(130, 30, title, Font_16x26, OLIVE, CYAN);

	display_DrawFilledRectangle(40, 70, elementWidth, elementHeight, elementColor);
	const char* sens = "Sensitivity";
	display_WriteString(105, 75, sens, Font_11x18, BLUE, elementColor);

	display_DrawFilledRectangle(40, 120, elementWidth, elementHeight, elementColor);
	const char* pres = "Presets";
	display_WriteString(125, 125, pres, Font_11x18, BLUE, elementColor);

	display_DrawFilledRectangle(40, 170, elementWidth, elementHeight, elementColor);
	const char* darkscan = "Dark Scan elim.";
	display_WriteString(85, 175, darkscan, Font_11x18, BLUE, elementColor);

	//select the first item
	MenuItemReset();
	display_DrawHollowRectangle(39, 69, elementWidth+1, elementHeight+1, RED);
}


void mainMenu()
{
	MenuItemReset();
	drawMainMenu();

	while(1)
	{
		if(BtnOk)
		{
			BtnOk = false;
			drawCurrentMenuMain();
		}
		else if(BtnUp)
		{
			BtnUp = false;
			unselectCurrentMenu();
			MenuItemUp();
			selectCurrentMenu();
		}
		else if(BtnDown)
		{
			BtnDown = false;
			unselectCurrentMenu();
			MenuItemDown(MainMenuMaxNum);
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


void drawCurrentMenuMain()
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
	SensitivityMenu sensMenu;
    sensMenu.show();
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


