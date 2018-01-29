/*
 * Sprite.h
 *
 * Created: 22.12.2017 17:02:59
 *  Author: Joerg
 */ 


#ifndef SPRITE_H_
#define SPRITE_H_

#include <avr/pgmspace.h>

struct SPRITE
{
  //! the position offset
  int8_t offset;
  //! the width in bits
  uint8_t width;
  //! the sprite type, 0= Hour, 1= Minute, 2= Second
  uint8_t type;

  const uint8_t *data PROGMEM;
};

extern const SPRITE sprites[] PROGMEM;
extern const uint8_t SPRITES;
extern const uint8_t dials[] PROGMEM;
extern const uint8_t DIALS;

#define FACES (SPRITES/3)

void drawDial(uint8_t dialIndex);
void drawTime(uint8_t spriteIndex, uint8_t hour, uint8_t minute, uint8_t second);

// index of dial shown in Display
extern uint8_t dialIndex;
// index of face shown in Display
extern uint8_t faceIndex;


#endif /* SPRITE_H_ */