#include <stdlib.h>

#include "sensitivity_menu.h"
#include "menu_misc.h"
#include "display.h"
#include "../Gen/settings.h"


SensitivityMenu::SensitivityMenu()
{
	int val = SpecDelay;
	int i = m_SensArrSize-1;

	while(val > 0 && i >=0)
	{
		m_sensArr[i] = val % 10;
		val  =  val/10;
		--i;
	}
}


void SensitivityMenu::selectCurrentItem()
{
	if(CurrentMenuPos >= m_menuItemNum)
		return; // todo: handle an error!

	if(m_menuItemNum-1 == CurrentMenuPos) // Save button
	{
		display_DrawHollowRectangle(m_x0+ m_offsetArr[CurrentMenuPos] -1, m_y0-10, 65, 40, RED);
	}
	else
	{
		display_DrawHollowRectangle(m_x0+ m_offsetArr[CurrentMenuPos] -2, m_y0-10, 20, 40, RED);
	}
}


void SensitivityMenu::unselectCurrentItem()
{
	if(CurrentMenuPos >= m_menuItemNum)
		return; // todo: handle an error!

	if(m_menuItemNum-1 == CurrentMenuPos) // Save button
	{
		display_DrawHollowRectangle(m_x0+ m_offsetArr[CurrentMenuPos] -1, m_y0-10, 65, 40, MenuButtonColor);
	}
	else
	{
		display_DrawHollowRectangle(m_x0+ m_offsetArr[CurrentMenuPos] -2, m_y0-10, 20, 40, WHITE);
	}
}



void SensitivityMenu::show()
{
	MenuItemReset();
	drawMenu();

	while(1)
	{
		if(BtnOk)
		{
			BtnOk = false;

			if(m_menuItemNum-1 == CurrentMenuPos) // Save button
			{
				setNewSensitivity();

				HAL_Delay(500);
				unselectCurrentItem();
				MenuItemReset();
				HAL_Delay(500);
				selectCurrentItem();
			}
			else
				editNum();
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
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
//			HAL_Delay(500);
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
//			HAL_Delay(500);

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


void SensitivityMenu::drawMenu()
{
	display_DrawFilledRectangle(20, 20, 280, 200, CYAN);
	const char* title = "Sensitivity";
	display_WriteString(70, 30, title, Font_16x26, OLIVE, CYAN);

	display_DrawFilledRectangle(m_x0-10, m_y0-10, 100, 40, WHITE);


	int xOff = 0;
	for(int i=0; i<m_SensArrSize; ++i)
	{
		char s[2] = {'a', '\0'};
		itoa(m_sensArr[i], s, 10);

		xOff = m_offsetArr[i];
		display_WriteString(m_x0+xOff, m_y0, s, Font_16x26, BLACK, WHITE);
	}

	xOff = m_offsetArr[4];
	//draw Save button
	display_DrawFilledRectangle(m_x0+xOff, m_y0-10, 65, 40, MenuButtonColor);
	display_WriteString(m_x0+xOff+10, m_y0, "Save", Font_11x18, BLUE, MenuButtonColor);


	display_WriteString(m_x0+10, m_y0+80, "+ <---> -", Font_16x26, BLACK, CYAN);

	//select default item
	display_DrawHollowRectangle(m_x0-2, m_y0-10, 20, 40, RED);
}


void SensitivityMenu::setNewSensitivity()
{
	SpecDelay = m_sensArr[0]*1000 + m_sensArr[1]*100 + m_sensArr[2]*10 + m_sensArr[3];
	//todo: save value to EEPROM
}


void SensitivityMenu::editNum()
{
	arrow(CurrentMenuPos, true); // draw an arrow

	while(1)
	{
		if(BtnOk)
		{
			BtnOk = false;
			break;
		}
		else if(BtnUp)
		{
			BtnUp = false;
			numberInc(CurrentMenuPos);
		}
		else if(BtnDown)
		{
			BtnDown = false;
			numberDec(CurrentMenuPos);
		}

		if(!MainMenuActive)
		{
			const char* title = "Exiting ...";
			display_WriteString(90, 100, title, Font_16x26, RED, CYAN);
			break;
		}
	}//while

	arrow(CurrentMenuPos, false);//erase an arrow
}


void SensitivityMenu::numberInc(const int pos)
{
	if(pos < 0 || pos >= m_SensArrSize)
		return; //todo: handle an error

	int a =	m_sensArr[pos];
	if(a >= 9) // must be from 0 to 9
		return;

	//increase a number and display it
	++a;
	m_sensArr[pos] = a;
	char s[2] = {'a', '\0'};
	itoa(a, s, 10);

	int xOff = m_offsetArr[pos];
	display_WriteString(m_x0+xOff, m_y0, s, Font_16x26, BLACK, WHITE);
}

void SensitivityMenu::numberDec(const int pos)
{
	if(pos < 0 || pos >= m_SensArrSize)
		return; //todo: handle an error

	int a =	m_sensArr[pos];
	if(a <= 0) // must be from 0 to 9
		return;

	//decrease a number and display it
	--a;
	m_sensArr[pos] = a;
	char s[2] = {'a', '\0'};
	itoa(a, s, 10);

	int xOff = m_offsetArr[pos];
	display_WriteString(m_x0+xOff, m_y0, s, Font_16x26, BLACK, WHITE);

}


void SensitivityMenu::arrow(const int pos, bool flag)
{
	uint16_t color = CYAN;
	if(true == flag)
		color = RED;

	int xOff = m_offsetArr[pos]+3;
	int yOff = m_y0 + 40;
	display_WriteString(m_x0+xOff, yOff, "^", Font_11x18, color, CYAN);
	display_WriteString(m_x0+xOff, yOff+10, "^", Font_11x18, color, CYAN);
}
