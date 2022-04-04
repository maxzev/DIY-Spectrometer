#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "stdbool.h"

//menu flags
extern bool MainMenuActive;
extern bool MenuOk;
extern bool MenuUp;
extern bool MenuDown;

extern int CurrentMenuPos;


void menuItemUp();
void menuItemDown();
void menuItemReset();
void selectCurrentMenu();
void unselectCurrentMenu();

void mainMenu();
void drawMainMenu();
void drawCurrentMenu();

void sensitivityMenu();
void presetsMenu();
void darkScanMenu();


#endif /* MAINMENU_H_ */
