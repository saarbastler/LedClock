/*
 * Sprite.cpp
 *
 * Created: 22.12.2017 17:06:58
 *  Author: Joerg
 */ 

#include "Sprite.h"

#include "LedUhr.h"

void drawSprite( const SPRITE *sprite, uint8_t position)
{
  //if( spriteIndex < SPRITES)
  {
    //const SPRITE *sprite= &sprites[spriteIndex];
    int8_t pos= position + pgm_read_byte(&sprite->offset);
    if( pos < 0 )
      pos += 60;
    pos %= 60;

    uint8_t width= pgm_read_byte(&sprite->width);
    uint8_t *ptr= (uint8_t*)pgm_read_word(&sprite->data);

    for(uint8_t i=0;i < width;i++)    
    {
      uint8_t data= pgm_read_byte(ptr++);
      uint8_t mask= pgm_read_byte(ptr++);

      for(uint8_t ring=0;ring < 8;ring++ )
      {
        if( data & 1 )
          LedUhr::led(ring, pos, 1);
        else if( mask & 1 )
          LedUhr::led(ring, pos, 0);

        mask >>= 1;
        data >>= 1;
      }

      pos++;
      if( pos > 59 )
        pos= 0;
    }
  }
}

void drawTime(uint8_t spriteIndex, uint8_t hour, uint8_t minute, uint8_t second)
{
  uint8_t hms[3];

  hms[0]= (hour % 12) * 5 + (minute + 6) / 12;
  hms[1]= minute;
  hms[2]= second;

  //LedUhr::clear();
  const SPRITE *sprite= &sprites[ 3 * spriteIndex];
  for(uint8_t i=0;i < 3;i++, sprite++)
  {
    uint8_t type= pgm_read_byte( &sprite->type);
    if( type < 3 )
      drawSprite(sprite, hms[type]);
  }
}

void drawDial(uint8_t dialIndex)
{
  if( dialIndex < DIALS)
  {
    const uint8_t *ptr= &dials[dialIndex*60];

    for(uint8_t i=0;i < 60;i++)
    {
      uint8_t data= pgm_read_byte(ptr++);
      for(uint8_t ring= 0;ring < 8;ring++,data >>= 1)
        LedUhr::led(ring, i, data & 1);
    }
  }

}
