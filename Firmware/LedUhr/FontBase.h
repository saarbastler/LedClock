#ifndef FONTBASE_INCLUDED
#define FONTBASE_INCLUDED

#include <avr/pgmspace.h>

class FontBase
{
public:

	virtual uint8_t getCharWidth() = 0;
	virtual uint8_t getCharHeight() = 0;

	virtual char getFirstChar() = 0;
	virtual char getLastChar() = 0;

	virtual PGM_P getData() = 0;
	virtual uint8_t getBytesPerColumn() = 0;
};

#endif
