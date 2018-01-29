/*
 * LedUhr.cpp
 *
 * Created: 16.12.2017 18:41:35
 *  Author: Joerg
 */ 

#include "LedUhr.h"

#include "saba_timer.h"
#include "Font5x7.h"

namespace LedUhr
{
  const uint8_t BITSHIFT[8] PROGMEM = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 };

  Font5x7 font;
  
  static const uint8_t DIGITS[10] PROGMEM = { A|B|C|D|E|F, B|C, A|B|D|E|G, A|B|C|D|G, F|B|G|C, A|F|G|C|D, A|F|G|C|D|E, A|B|C, A|B|C|D|E|F|G, A|B|C|D|F|G };

  static uint8_t doubleBufferIndex;
  static uint8_t index;
  static uint8_t displayRam[2][8][8];

  static uint8_t *getDisplayRam()
  {
    if(doubleBufferIndex)
      return displayRam[1][0];
    else
      return displayRam[0][0];
  }

  static uint8_t *getDisplayRam(uint8_t ring)
  {
    return getDisplayRam() + (ring & 7) * 8;
  }


  void toggleRam()
  {
    doubleBufferIndex= !doubleBufferIndex;
  }

  void init()
  {
    DATAIN datain;
    datain.asOutput()= false;
    CLOCK clock;
    clock.asOutput()= false;
    STROBE strobe;
    strobe.asOutput()= false;

    ROW row;
    row.ddr= 0xff;
    row.port= 0xff;

    index= 0;
    doubleBufferIndex= 0;
    clear();
    toggleRam();
    clear();

    SABA::Timer0 timer0;
    timer0.waveformGenerationMode(SABA::Timer8::Normal)
    .clockSelect(SABA::Timer8::By64)
    .enableOverflowInterrupt(true)
    .enableOutputCompAMatchInterrupt(true)
    .ocra= 0x30;
  }

  uint8_t bitshift(uint8_t index)
  {
    return pgm_read_byte(&BITSHIFT[index & 7]);
  }

  static void selectRing( uint8_t ring)
  {
    ROW row;
    ring &= 7;

    row.port= ~bitshift(ring);
  }

  static void off( )
  {
    ROW row;
    row.port= 0xff;
  }

  static void shiftout()
  {
    off();

    DATAIN datain;
    CLOCK clock;

    uint8_t *ptr;
    if(doubleBufferIndex)
      ptr= displayRam[0][index];
    else
      ptr= displayRam[1][index];

    for(uint8_t b= 0;b < 8;b++)
    {
      uint8_t data= *ptr++;
      for(uint8_t i=0;i < 8; i++,data >>= 1)
      {
        datain= (data & 1) != 0;
        clock= true;
        clock= false;
      }
    }

    STROBE strobe;
    strobe= true;
    strobe= false;

    selectRing(index);
    index = (index +1 ) & 0x7;
  }

  ISR(TIMER0_OVF_vect)
  {
    shiftout();
  }

  ISR(TIMER0_COMPA_vect)
  {
    off();
  }

  void clear()
  {
    memset(getDisplayRam(), 0, sizeof(displayRam)/2);
  }

  void led( uint8_t ring, uint8_t index, bool on)
  {
    if( index < 60)
    {
      uint8_t *ptr= getDisplayRam(ring);

      if( index > 4 )
        index += 4;
      index = (index - 5) & 63;

      ptr += (index >> 3);
      if( on )
        *ptr |= bitshift(index);
      else
        *ptr &= ~bitshift(index);
    }
  }

  void string(uint8_t position, const char *string)
  {
    for(uint8_t i=0;i < 10;i++,position+= 6)
    {
      char ch= pgm_read_byte(string++);
      if( ch == 0 )
        break;
      
      putchar(position, ch);
    }
    
  }
  
  void putchar(uint8_t position, char ch)
  {
    if( ch >= font.getFirstChar())
    {
      PGM_P cp= font.getData() + uint8_t(ch - font.getFirstChar()) * (font.getCharWidth() * font.getBytesPerColumn());
    
      position %= 60;
      for(uint8_t x=0;x < font.getCharWidth();x++)
      {
        uint8_t bits= pgm_read_byte(cp++);
        for(uint8_t y=0;y < font.getCharHeight();y++)
        {
           bits <<= 1;
           led(y, position, bits & 0x80);
        }
        ++position;
        if( position > 59)
          position= 0;
      }
    }    
  }
  
  /*void setRam(uint8_t ring, uint8_t index, uint8_t value)
  {
    displayRam[ring & 7][index & 7]= value;
  }

  uint8_t getRam(uint8_t ring, uint8_t index)
  {
    return displayRam[ring & 7][index & 7];
  }*/

  void digitChar( uint8_t pos, uint8_t digit)
  {
    if( pos < 4 && digit < 10 )
      digitBits(pos, pgm_read_byte(&DIGITS[digit]));
  }

  void digitBits( uint8_t pos, uint8_t bits)
  {
    if( pos < 4)
    {
      uint8_t mask= 1 << pos;
      uint8_t *ptr= getDisplayRam();

      for(uint8_t i=0;i < 8;i++)
      {
        if( bits & 1 )
          *ptr |= mask;
        else
          *ptr &= ~mask;

        ptr += 8;
        bits >>= 1;
      }
    }
  }

  void decimalPoint(uint8_t pos, bool on)
  {
    if( pos < 4 )
    {
      uint8_t mask= bitshift(pos);
      uint8_t *ptr= getDisplayRam(DP_POS);

      if( on )
        *ptr |= mask;
      else
        *ptr &= ~mask;
    }
  }

  void digitNumber( uint16_t value )
  {
    for(uint8_t i = 0;i < 4; i++)
    {
      digitChar( i, value % 10);
      value /= 10;
    }
  }
}
