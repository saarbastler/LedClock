/*
 * eeprom.cpp
 *
 * Created: 21.01.2018 15:05:33
 *  Author: Joerg
 */ 

#include <avr/eeprom.h>

#include <saba_timer.h>

#include "Sprite.h"

void persistConfiguration()
{
  uint8_t *address= 0;
  eeprom_write_byte(address++, 'S');
  eeprom_write_byte(address++, 'A');
  eeprom_write_byte(address++, 'B');
  eeprom_write_byte(address++, 'A');
  
  eeprom_write_byte(address++, dialIndex);
  eeprom_write_byte(address++, faceIndex);

  SABA::Timer0 timer0;
  eeprom_write_byte(address++, timer0.ocra());
}

bool readConfiguration()
{
  uint8_t *address= 0;
  uint8_t v= eeprom_read_byte(address++);
  if(v != 'S')
    return false;
  v= eeprom_read_byte(address++);
  if(v != 'A')
    return false;
  v= eeprom_read_byte(address++);
  if(v != 'B')
    return false;
  v= eeprom_read_byte(address++);
  if(v != 'A')
    return false;
  
  v= eeprom_read_byte(address++);
  if( v < DIALS )
    dialIndex= v;
    
  v= eeprom_read_byte(address++);
  if( v < FACES )
    faceIndex= v;
  
  v= eeprom_read_byte(address++);
  if( v >= 0x14 )
  {
    SABA::Timer0 timer0;
    timer0.ocra= v;
  }  
}