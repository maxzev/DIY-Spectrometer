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

	const int m_menuItemNum = 3; //currently, there are 3 menu items on every screen
	uint16_t  m_elementWidth  = 240;
	uint16_t  m_elementHeight = 30;

	void drawMenu();
	void drawCurrentMenuItem();

	void sensitivityMenu();
	void presetsMenu();
	void pcCommandModeMenu();  // get commands from PC application

	void selectCurrentItem();
	void unselectCurrentItem();

};


#endif /* MAINMENU_H_ */
