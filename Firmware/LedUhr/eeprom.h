/*
 * eeprom.h
 *
 * Created: 21.01.2018 15:18:51
 *  Author: Joerg
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

/** 
  * read the configurration from eeprom
  * @return true if eeprom data is valid
  */
bool readConfiguration();

/**
  * write the configuration to the eeprom
  */
void persistConfiguration();



#endif /* EEPROM_H_ */