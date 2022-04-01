#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "stdbool.h"


extern bool MainMenuActive;
extern int CurrentMenuPos;


void menuItemUp();
void menuItemDown();
void selectCurrentMenu();
void unselectCurrentMenu();

void mainMenu();
void drawCurrentMenu();

void sensitivityMenu();
void presetsMenu();
void darkScanMenu();


#endif /* MAINMENU_H_ */
