#ifndef IO_CONFIG_H_
#define IO_CONFIG_H_

#include <saba.h>

// put your IO configuration here, example:
//typedef SABA::PortPin<(SABA::SFRA)&PINC,3> LED;

typedef SABA::PortPin<(SABA::SFRA)&PINC,0> DATAIN;
typedef SABA::PortPin<(SABA::SFRA)&PINC,1> CLOCK;
typedef SABA::PortPin<(SABA::SFRA)&PINC,2> STROBE;

typedef SABA::Port8<(SABA::SFRA)&PINB> ROW;
//typedef SABA::PortRange<(SABA::SFRA)&PINB,0x3f, 0> ROW05;
//typedef SABA::PortPin<(SABA::SFRA)&PIND,6> ROW6;
//typedef SABA::PortPin<(SABA::SFRA)&PIND,7> ROW7;

// Stecker K3, Pin1 = GND
typedef SABA::PortPin<(SABA::SFRA)&PIND,4> K32;
typedef SABA::PortPin<(SABA::SFRA)&PIND,3> K33;
typedef SABA::PortPin<(SABA::SFRA)&PIND,2> K34;

// Stecker K6, Pin4 = GND
typedef SABA::PortPin<(SABA::SFRA)&PIND,7> K41;
typedef SABA::PortPin<(SABA::SFRA)&PIND,6> K42;
typedef SABA::PortPin<(SABA::SFRA)&PIND,5> K43;

constexpr uint8_t PHASE1_BIT= 5;
constexpr uint8_t PHASE2_BIT= 6;
constexpr uint8_t BUTTON_BIT= 7;

#endif /* IO_CONFIG_H_ */