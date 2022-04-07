#include <stdlib.h>

#include "sensitivity_menu.h"
#include "menu_misc.h"
#include "display.h"
#include "../Gen/settings.h"


SensitivityMenu::SensitivityMenu()
{
}


void SensitivityMenu::selectCurrentItem()
{
	if(CurrentMenuPos >= SensMenuMaxNum)
		return; // todo: handle an error!

	uint16_t arr [SensMenuMaxNum] = {0, 20, 40, 60};
	display_DrawHollowRectangle(x0+ arr[CurrentMenuPos] -2, y0-10, 20, 40, RED);
}


void SensitivityMenu::unselectCurrentItem()
{
	if(CurrentMenuPos >= SensMenuMaxNum)
		return; // todo: handle an error!

	uint16_t arr [SensMenuMaxNum] = {0, 20, 40, 60};
	display_DrawHollowRectangle(x0+ arr[CurrentMenuPos] -2, y0-10, 20, 40, WHITE);
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
			//drawCurrentSens();
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
			MenuItemDown(SensMenuMaxNum);
			selectCurrentItem();
		}

		if(!MainMenuActive)
		{
			const char* title = "Exiting ...";
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

	int val = SpecDelay;
	const int N = 4;
	int arr[N] = {0,0,0,0};

	int i = N-1;
	while(val > 0 && i >=0)
	{
		arr[i] = val % 10;
		val  =  val/10;
		--i;
	}

	display_DrawFilledRectangle(x0-10, y0-10, 100, 40, WHITE);

	uint16_t xOff = 0;
	for(int i=0; i<N; ++i)
	{
		char s[2] = {'a', '\0'};
		itoa(arr[i], s, 10);

		display_WriteString(x0+xOff, y0, s, Font_16x26, BLACK, WHITE);
		xOff += 20;
	}

	display_WriteString(x0-10, y0+50, "<---->", Font_16x26, BLACK, CYAN);

	//select default digit
	display_DrawHollowRectangle(x0-2, y0-10, 20, 40, RED);
}


