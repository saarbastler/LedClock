/*
 * Menu.h
 *
 * Created: 20.01.2018 20:08:25
 *  Author: Joerg
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "application.h"
#include "LedUhr.h"
#include "Sprite.h"
#include "eeprom.h"

extern uint8_t daysOfMonth(uint8_t month, uint8_t year);

class Menu
{
public:  

  /** 
    * the user turned the encoder in that activated menu 
    * @param counter: absolute value
    * @apram increment: the increment value
    */
  virtual void turn (int8_t counter, int8_t increment )  { }
  
  /** 
    * the user clicked the encoder in that activated menu. The default behaviour is to save and leave the menu
    * @param down: true, if button is down, false if up
    */
  virtual void click(bool down)
  {
    if(down)
    {
      saveValue();
      mode &= ~SABA::BIT(MODE_MENU_ACTIVE);
    }      
  }

  /**
    * paint the menu
    * @param offset: position to paint to, used for slow rotating
    */
  virtual void paintMenu(uint8_t offset)  { }

  /**
    * set the static value field on entering the menu
    */
  virtual void setValue() { }
      
  /**
    * save the static value field on leaving the menu
    */
  virtual void saveValue() { }
    
  /**
    * show the value
    */
  virtual void showValue() { }
  
  /**
    * activate that menu, the default will paint the menu and, set the value and show it
    */
  virtual void activate()
  {
    paintMenu(0);      
    setValue();
    showValue();
  }

  /**
    * Helper method used to increment/decrement the value
    * @param increment: positive for increment, negative for decrement
    * @param the max value
    */
  void turnMax(int8_t increment, uint8_t max, uint8_t min= 0)
  {
    if( increment < 0 )
    {
      --value;
      if( int8_t(value) < min)
        value= max;
    }
    else
    {
      ++value;
      if( value > max || value < min)
        value= min;
    }
    showValue();
  }
  
protected:

  static uint8_t value;    
};

class MenuHours : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-21,PSTR("Stunden"));    
  }

  virtual void setValue()
  {
    value= ds1307.getHours();
  }
  
  virtual void saveValue()
  {
    ds1307.setHours(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 23);
  }

  void showValue()
  {
    LedUhr::digitChar(3, value / 10);
    LedUhr::digitChar(2, value % 10);    
  }
};

class MenuMinutes : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-21,PSTR("Minuten"));
  }
  
  virtual void setValue()
  {
    value= ds1307.getMinutes();
  }
  
  virtual void saveValue()
  {
    ds1307.setMinutes(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 59);
  }

  void showValue()
  {
    LedUhr::digitChar(1, value / 10);
    LedUhr::digitChar(0, value % 10);
  }
};

class MenuSeconds : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-24,PSTR("Sekunden"));
  }
  
  virtual void setValue()
  {
    value= ds1307.getSeconds();
  }
  
  virtual void saveValue()
  {
    ds1307.setSeconds(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 59);
  }

  void showValue()
  {
    LedUhr::digitChar(3, 5);
    LedUhr::digitChar(1, value / 10);
    LedUhr::digitChar(0, value % 10);
  }
};

class MenuDate : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-9,PSTR("Tag"));
  }
  
  virtual void setValue()
  {
    value= ds1307.getDate();
  }
  
  virtual void saveValue()
  {
    ds1307.setDate(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, daysOfMonth(ds1307.getMonth(), ds1307.getYear()), 1);
  }

  void showValue()
  {
    LedUhr::digitChar(1, value / 10);
    LedUhr::digitChar(0, value % 10);
  }
};

class MenuMonth : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-15,PSTR("Monat"));
  }
  
  virtual void setValue()
  {
    value= ds1307.getMonth();
  }
  
  virtual void saveValue()
  {
    ds1307.setMonth(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 12, 1);
  }

  void showValue()
  {
    LedUhr::digitChar(1, value / 10);
    LedUhr::digitChar(0, value % 10);
  }
};

class MenuYear : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-12,PSTR("Jahr"));
  }
  
  virtual void setValue()
  {
    value= ds1307.getYear();
  }
  
  virtual void saveValue()
  {
    ds1307.setYear(value);
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 99);
  }

  void showValue()
  {
    LedUhr::digitChar(3, 2);
    LedUhr::digitChar(2, 0);
    LedUhr::digitChar(1, value / 10);
    LedUhr::digitChar(0, value % 10);
  }
};

class MenuFace : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-18,PSTR("Zeiger"));
  }
  
  virtual void setValue()
  {
    value= faceIndex;
  }
  
  virtual void saveValue()
  {
    faceIndex= value;
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, FACES-1);
    showValue();
  }

  void showValue()
  {
    LedUhr::clear();
    drawTime(value, ds1307.getHours(), ds1307.getMinutes(), ds1307.getSeconds());
  }
};

class MenuDial : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-30,PSTR("Ziff.blatt"));
  }
  
  virtual void setValue()
  {
    value= dialIndex;
  }
  
  virtual void saveValue()
  {
    dialIndex= value;
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, DIALS-1);
    showValue();
  }

  void showValue()
  {
    LedUhr::clear();
    drawDial(value);
  }
};

class MenuBrightness : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-30,PSTR("Helligkeit"));
  }
  
  virtual void setValue()
  {
    SABA::Timer0 timer0;
    value= timer0.ocra();
  }
  
  virtual void saveValue()
  {
    SABA::Timer0 timer0;
    timer0.ocra= value;
  }
  
  virtual void turn (int8_t counter, int8_t increment )
  {
    turnMax(increment, 0xff, 0x14);
    showValue();
  }

  void showValue()
  {
    SABA::Timer0 timer0;
    timer0.ocra= value;
    
    LedUhr::digitChar(2, value / 100);
    uint8_t v= value % 100;
    LedUhr::digitChar(1, v / 10);
    LedUhr::digitChar(0, v % 10);
  }
};

class MenuEnd : public Menu
{
  virtual void paintMenu(uint8_t offset)
  {
    LedUhr::string(offset+60-12,PSTR("ENDE"));
  }
  
  virtual void activate()
  {
    persistConfiguration();
    mode &= ~(SABA::BIT(MODE_MENU)|SABA::BIT(MODE_MENU_ACTIVE));
  }
};

#endif /* MENU_H_ */