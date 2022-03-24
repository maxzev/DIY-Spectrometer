#include "stdint.h"
#include "menu_misc.h"


bool MainMenuActive = false;
bool BtnOk   = false;
bool BtnUp   = false;
bool BtnDown = false;

int CurrentMenuPos = 0;
const uint16_t MenuButtonColor  = 0xEDEA;



void MenuItemUp()
{
	if(0 == CurrentMenuPos)
		return;

	--CurrentMenuPos;
}


void MenuItemDown(const int maxPos)
{
	if(CurrentMenuPos < maxPos-1)
		++CurrentMenuPos;
}


void MenuItemReset()
{
	CurrentMenuPos = 0;
}
