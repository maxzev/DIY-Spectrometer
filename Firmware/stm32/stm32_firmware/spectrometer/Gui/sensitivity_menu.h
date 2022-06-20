#ifndef SENSITIVITY_MENU_H_
#define SENSITIVITY_MENU_H_

#include <stdint.h>

class SensitivityMenu
{
public:
	SensitivityMenu();
	SensitivityMenu(const SensitivityMenu &) = delete;
	SensitivityMenu & operator=(const SensitivityMenu &) = delete;

	void show();


private:

	const int m_menuItemNum = 5; // zero based index
	const uint16_t m_x0 = 80;
	const uint16_t m_y0 = 100;
	uint16_t m_offsetArr [5] = {0, 20, 40, 60, 80};

	static constexpr int m_SensArrSize = 4;
	int m_sensArr[m_SensArrSize] = {0,0,0,0};

	void drawMenu();
	void selectCurrentItem();
	void unselectCurrentItem();

	void setNewSensitivity();
	void editNum();

	void arrow(const int pos, bool flag); //draw an arrow pointer under selected digit
	void numberInc(const int pos);
	void numberDec(const int pos);

};


#endif /* SENSITIVITY_MENU_H_ */
