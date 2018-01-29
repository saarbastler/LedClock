/*
 * application.h
 *
 * Created: 20.01.2018 20:13:22
 *  Author: Joerg
 */ 


#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "DS1307.h"

// The mode bit vector
extern uint8_t mode;
extern SABA::I2C::DS1307<0> ds1307;

// the Battery was empty, DS1307lost time/date information
#define MODE_DS1307_BATTERY_EMPTY     0
#define MODE_GREETING                 1
#define MODE_MENU                     2
#define MODE_MENU_ACTIVE              3




#endif /* APPLICATION_H_ */