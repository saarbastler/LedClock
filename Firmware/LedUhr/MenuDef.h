/*
 * MenuDef.h
 *
 * Created: 21.01.2018 11:44:27
 *  Author: Joerg
 */ 


#ifndef MENUDEF_H_
#define MENUDEF_H_

#include "Menu.h"

extern uint8_t menuIndex;
extern uint8_t menuTimeout;

extern const Menu *const menus[] PROGMEM;
extern Menu *currentMenu;

const uint8_t menuMax();

/**
  * return the max days of that month, uses a leap year algorithm
  * @param the month 1 .. 12
  * @param the year 00 .. 99
  */
uint8_t daysOfMonth(uint8_t month, uint8_t year);


#endif /* MENUDEF_H_ */