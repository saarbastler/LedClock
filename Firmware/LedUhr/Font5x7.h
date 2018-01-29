#ifndef FONT5X7_INCLUDED
#define FONT5X7_INCLUDED

#include "FontBase.h"

class Font5x7 : public FontBase
{
public:

	virtual uint8_t getCharWidth()					{ return 5; }
	virtual uint8_t getCharHeight()				{ return 7; }

	virtual char getFirstChar() 				{ return ' '; }
	virtual char getLastChar()					{ return 0xff; }

	virtual PGM_P getData() 						{ return (PGM_P)fontData; }
	virtual uint8_t getBytesPerColumn()		{ return 1; }

private:

	static const uint8_t fontData[] PROGMEM;
};


#endif
