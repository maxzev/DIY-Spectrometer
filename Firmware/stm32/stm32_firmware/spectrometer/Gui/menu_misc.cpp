#include "stdint.h"
#include "menu_misc.h"


bool MainMenuActive = false;
bool BtnOk   = false;
bool BtnUp   = false;
bool BtnDown = false;

int CurrentMenuPos = 0;


void menuItemUp()
{
	if(0 == CurrentMenuPos)
		return;

	--CurrentMenuPos;
}


void menuItemDown(const int maxPos)
{
	if(CurrentMenuPos < maxPos-1)
		++CurrentMenuPos;
}


void menuItemReset()
{
	CurrentMenuPos = 0;
}
