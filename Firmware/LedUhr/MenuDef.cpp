/*
 * Menu.cpp
 *
 * Created: 21.01.2018 11:43:07
 *  Author: Joerg
 */ 

#include "MenuDef.h"

uint8_t menuIndex;
uint8_t menuTimeout;

const MenuHours menuHours;
const MenuMinutes menuMinutes;
const MenuSeconds menuSeconds;
const MenuDate menuDate;
const MenuMonth menuMonth;
const MenuEnd menuEnd;
const MenuYear menuYear;
const MenuFace menuFace;
const MenuDial menuDial;
const MenuBrightness menuBrightness;
uint8_t Menu::value;

const Menu *const menus[] PROGMEM= {&menuHours,&menuMinutes,&menuSeconds,&menuDate,&menuMonth,&menuYear,&menuFace,&menuDial,&menuBrightness,&menuEnd};
Menu *currentMenu;

#define MENU_MAX            (sizeof(menus)/sizeof(menus[0]))

const uint8_t menuMax()
{
  return MENU_MAX;
}

static const uint8_t days[] PROGMEM = {31,28,31,30,31,30,31,31,30,31,30,31};

uint8_t daysOfMonth(uint8_t month, uint8_t year)
{
  if(month == 2)
  {
    return ( (year & 0x3) == 0) ? 29 : 28;
  }
  else if( month > 0 && month < 13)
  {
    return pgm_read_byte(&days[month-1]);
  }
  
  return 0;
  
}