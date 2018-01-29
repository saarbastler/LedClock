/*
 * LedUhr.h
 *
 * Created: 16.12.2017 18:38:43
 *  Author: Joerg
 */ 


#ifndef LEDUHR_H_
#define LEDUHR_H_

#include <string.h>

#include <avr/pgmspace.h>

#include <avr/interrupt.h>

#include "io_config.h"

/**
   @namespace LedUhr
   @brief LED display and multiplexing helper functions
*/
namespace LedUhr
{
  void init();

  /** clear the display, all leds off */
  void clear();
  
  /** turn the led on or off
    * @param ring: 0 (inner) .. 7 (outer) ring
    * @param pos: 0 .. 59 Seonds position
    * @param on: true: led on, false: led off
    */
  void led( uint8_t ring, uint8_t pos, bool on);

  // Ring Segment
  // 0 g     aaa
  // 1 b    f   b
  // 2 a    f   b
  // 3 f     ggg
  // 4 d    e   c
  // 5 dp   e   c
  // 6 e     ddd
  // 7 c          dp

  constexpr uint8_t G_POS= 7;
  constexpr uint8_t B_POS= 6;
  constexpr uint8_t A_POS= 5;
  constexpr uint8_t F_POS= 4;
  constexpr uint8_t D_POS= 3;
  constexpr uint8_t DP_POS= 2;
  constexpr uint8_t E_POS= 1;
  constexpr uint8_t C_POS= 0;

  constexpr uint8_t A= _BV(A_POS);
  constexpr uint8_t B= _BV(B_POS);
  constexpr uint8_t C= _BV(C_POS);
  constexpr uint8_t D= _BV(D_POS);
  constexpr uint8_t E= _BV(E_POS);
  constexpr uint8_t F= _BV(F_POS);
  constexpr uint8_t G= _BV(G_POS);
  constexpr uint8_t DP= _BV(DP_POS);

  /** show a digit at 7 Segment display
    * @param pos= 0..3 right to left, other values are ignored
    * @param digit= 0..9 digit to show, other values are ignored
    */
  void digitChar( uint8_t pos, uint8_t digit);

  /** show a bitvector at 7 Segment display
    * @param pos= 0..3 right to left, other values are ignored
    * @param digit= 0..FF, see constants A..DP
    */
  void digitBits( uint8_t pos, uint8_t digit);

  /** the 16 bit value 0.9999 is shown on the display, higher values are ignored
    * @param values to show
    */
  void digitNumber( uint16_t value );

  /** turn decimal point on or off
    * @param pos= 0..3 left to right, other values are ignored
    * @param on: true= on, false= off
    */
  void decimalPoint(uint8_t pos, bool on);

  //void setRam(uint8_t ring, uint8_t index, uint8_t value);
  //uint8_t getRam(uint8_t ring, uint8_t index);

  /** same as 1 << index, but read from ROM
    * @param index= 0..7, index is "anded" with 7
    * @return bit index set
    */
  uint8_t bitshift(uint8_t index);

  /** print a 5x7 Font char at position
    * @param position: position to print char, 0..59 modulo 60 is done in the function
    * @char character to print blank to 0xff
    */
  void putchar(uint8_t position, char ch);

  /** print a Rom (PSTR) String at position, max 10 chars
    * @param position: position to print char, 0..59 modulo 60 is done in the function
    * @string PSTR string
    */
  void string(uint8_t position, const char *string);
  
  /** toggle the Display RAM. Should be called after changes in RAM to display it. Avoids flickering */
  void toggleRam();
};



#endif /* LEDUHR_H_ */