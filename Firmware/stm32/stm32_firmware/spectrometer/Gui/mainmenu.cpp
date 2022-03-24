#include "mainmenu.h"
#include "display.h"
#include "sensitivity_menu.h"



MainMenu::MainMenu()
{
}


void MainMenu::selectCurrentItem()
{
	if(CurrentMenuPos >= m_menuItemNum)
		return; // todo: handle an error!

	uint16_t arr [m_menuItemNum] = {69, 119, 169};
	display_DrawHollowRectangle(39, arr[CurrentMenuPos], m_elementWidth+1, m_elementHeight+1, RED);
}


void MainMenu::unselectCurrentItem()
{
	if(CurrentMenuPos >= m_menuItemNum)
		return; // todo: handle an error!

	uint16_t arr [m_menuItemNum] = {69, 119, 169};
	display_DrawHollowRectangle(39, arr[CurrentMenuPos], m_elementWidth+1, m_elementHeight+1, CYAN);
}


void MainMenu::drawMenu()
{
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "MENU";
	display_WriteString(130, 30, title, Font_16x26, OLIVE, CYAN);

	display_DrawFilledRectangle(40, 70, m_elementWidth, m_elementHeight, MenuButtonColor);
	const char* sens = "Sensitivity";
	display_WriteString(105, 75, sens, Font_11x18, BLUE, MenuButtonColor);

	display_DrawFilledRectangle(40, 120, m_elementWidth, m_elementHeight, MenuButtonColor);
	const char* pres = "Presets";
	display_WriteString(125, 125, pres, Font_11x18, BLUE, MenuButtonColor);

	display_DrawFilledRectangle(40, 170, m_elementWidth, m_elementHeight, MenuButtonColor);
	const char* darkscan = "PC command mode";
	display_WriteString(85, 175, darkscan, Font_11x18, BLUE, MenuButtonColor);

	//select the first item
	MenuItemReset();
	display_DrawHollowRectangle(39, 69, m_elementWidth+1, m_elementHeight+1, RED);
}


void MainMenu::show()
{
	MenuItemReset();
	drawMenu();

	while(1)
	{
		if(BtnOk)
		{
			BtnOk = false;
			drawCurrentMenuItem();
		}
		else if(BtnUp)
		{
			BtnUp = false;
			unselectCurrentItem();
			MenuItemUp();
			selectCurrentItem();
		}
		else if(BtnDown)
		{
			BtnDown = false;
			unselectCurrentItem();
			MenuItemDown(m_menuItemNum);
			selectCurrentItem();
		}

		if(!MainMenuActive)
		{
			const char* title = "Exiting ...";
			display_DrawFilledRectangle(60, 80, 200, 60, CYAN);
			display_WriteString(90, 100, title, Font_16x26, RED, CYAN);
			break;
		}
	}//while
}


void MainMenu::drawCurrentMenuItem()
{
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
		pcCommandModeMenu();
	}
}


void MainMenu::sensitivityMenu()
{
	SensitivityMenu sensMenu;
    sensMenu.show();
}


void MainMenu::presetsMenu()
{
	//todo: for test
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "Presets";
	display_WriteString(110, 30, title, Font_16x26, OLIVE, CYAN);

	//todo: coming soon
}


void MainMenu::pcCommandModeMenu()
{
	//todo: for test
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "PC command mode";
	display_WriteString(45, 30, title, Font_16x26, OLIVE, CYAN);

	//todo: coming soon
}


