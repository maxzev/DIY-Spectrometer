#ifndef MAINMENU_H_
#define MAINMENU_H_

#include "stdint.h"
#include "menu_misc.h"


class MainMenu
{
public:
	MainMenu();
	MainMenu(const MainMenu &) = delete;
	MainMenu & operator=(const MainMenu &) = delete;

	void show();


private:

	const int m_menuItemNum = 3; // there are 3 menu items per each screen currently
	uint16_t  m_elementWidth  = 240;
	uint16_t  m_elementHeight = 30;
	uint16_t  m_elementColor  = 0xEDEA;

	void drawMenu();
	void drawCurrentMenuItem();

	void sensitivityMenu();
	void presetsMenu();
	void darkScanMenu();

	void selectCurrentItem();
	void unselectCurrentItem();

};


//void mainMenu();
//void drawMainMenu();
//void drawCurrentMenuMain();
//
//void sensitivityMenu();
//void presetsMenu();
//void darkScanMenu();


#endif /* MAINMENU_H_ */
