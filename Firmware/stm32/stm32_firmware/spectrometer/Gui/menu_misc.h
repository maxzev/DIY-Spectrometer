#ifndef MENU_MISC_H_
#define MENU_MISC_H_

#include "stdbool.h"

//menu flags
extern bool MainMenuActive;
extern bool BtnOk;
extern bool BtnUp;
extern bool BtnDown;

extern int CurrentMenuPos;
extern const uint16_t  MenuButtonColor;


void MenuItemUp();
void MenuItemDown(const int maxPos);
void MenuItemReset();


#endif /* MENU_MISC_H_ */
