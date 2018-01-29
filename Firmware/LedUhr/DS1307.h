/*
 * DS1307.h
 *
 * Created: 20.12.2017 21:15:43
 *  Author: Joerg
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include "Device.h"

#include "saba_i2cm.h"

#ifdef DEBUG_DS1307
extern void putch(uint8_t c);
extern SABA::OStream <&putch> out;
#endif

namespace SABA
{
  namespace I2C
  {
    template<uint8_t EXTRA_SIZE>
    class DS1307
    {
    public:
      static constexpr uint8_t ADDRESS= 0xd0;

      typedef void (*DONE_FUNC)(void *env, bool success);

      DS1307(SABA::I2C::I2CMaster *master) : master(master) {}

      bool readDevice(DONE_FUNC doneFunc_ = nullptr, void *env_ = nullptr)
      {
        if(!master->operator ()())
        {
          doneFunc= doneFunc_;
          env= env_;

          buffer[0]= 0;


          return master->startWriteAndRead(uint8_t(ADDRESS), uint8_t(1), (uint8_t*) buffer, uint8_t(8 + EXTRA_SIZE), buffer+1, [](void *env, const CMD *cmd)
          {
            ((DS1307<EXTRA_SIZE>*)env)->dataAvailable= ( cmd->error == 0 );
            if(((DS1307<EXTRA_SIZE>*)env)->doneFunc)
              ((DS1307<EXTRA_SIZE>*)env)->doneFunc(((DS1307<EXTRA_SIZE>*)env)->env, cmd->error == 0);
          }, this);
        }

        return false;
      }

      bool writeDevice(DONE_FUNC doneFunc_ = nullptr, void *env_ = nullptr)
      {
        if(!master->operator ()())
        {
          doneFunc= doneFunc_;
          env= env_;

          buffer[0]= 0;

          return master->startWrite(uint8_t(ADDRESS), uint8_t(9 + EXTRA_SIZE), buffer, [](void *env, const CMD *cmd)
          {
            ((DS1307<EXTRA_SIZE>*)env)->modified= !( cmd->error == 0 );
            if(((DS1307<EXTRA_SIZE>*)env)->doneFunc)
              ((DS1307<EXTRA_SIZE>*)env)->doneFunc(((DS1307<EXTRA_SIZE>*)env)->env, cmd->error == 0);
          },this);
        }

        return false;
      }

      bool isModified()
      {
        return modified;
      }

      bool isDataAvailable()
      {
        return dataAvailable;
      }

      void resetDataAvailable()
      {
        dataAvailable= false;
      }

      bool isClockEnabled()
      {
        return (buffer[1] & 0x80) == 0;
      }

      void enableClock(bool enable)
      {
        if( enable )
          buffer[1] &= ~0x80;
        else
          buffer[1] |= 0x80;

        modified= true;
      }

      uint8_t getSeconds()
      {
        return 10 * ((buffer[1] & 0x70) >> 4) + (buffer[1] & 0xf);
      }

      void setSeconds( uint8_t v)
      {
        if( v < 60 )
        {
          uint8_t b= (uint8_t)((v / 10) << 4) & 0x70;
          v %= 10;
          b |= v;

          buffer[1]= (buffer[1] & 0x80) | b;
          modified= true;
        }
      }

      uint8_t getMinutes()
      {
        return 10 * ((buffer[2] & 0x70)>>4) + (buffer[2] & 0x0f);
      }

      void setMinutes( uint8_t v )
      {
        if( v < 60 )
        {
          buffer[2]= ((v / 10) << 4) | (v % 10);
          modified= true;
        }
      }

      uint8_t getHours()
      {
        return 10 * ((buffer[3] & 0x30 ) >> 4) + (buffer[3] & 0x0f);
      }

      void setHours( uint8_t v )
      {
        if( v < 24 )
        {
          buffer[3]= ((v / 10) << 4) | (v % 10);
          modified= true;
        }
      }

      uint8_t getDay()
      {
        return buffer[4] & 7;
      }

      void setDay(uint8_t day)
      {
        buffer[4]= day & 7;
        modified= true;
      }

      uint8_t getDate()
      {
        return 10 * ((buffer[5] & 0x30) >> 4) + (buffer[5] & 0x0f);
      }

      void setDate( uint8_t v )
      {
        if( v > 0 && v < 32 )
        {
          buffer[5]= ((v / 10) << 4) | (v % 10);
          modified= true;
        }
      }

      uint8_t getMonth()
      {
        return ((buffer[6] & 0x10) ? 10 : 0) + (buffer[6] & 0x0f);
      }

      void setMonth( uint8_t v )
      {
        if( v > 0 && v < 13 )
        {
          buffer[6]= ((v > 9) ? 0x10 : 0) | (v % 10);
          modified= true;
        }
      }

      uint8_t getYear()
      {
        return 10 * ((buffer[7] & 0xf0) >> 4) + (buffer[7] & 0x0f);
      }

      void setYear( uint8_t v )
      {
        if( v > 0 && v < 100 )
        {
          buffer[7]= ((v / 10) << 4) | (v % 10);
          modified= true;
        }
      }
      uint16_t getHoursMinutes()
      {
        return 1000 * ((buffer[3] & 0x30)>>4) + 100 *(buffer[3] & 0xf) + 10 * ((buffer[2] & 0x70)>>4) + (buffer[2] & 0x0f);
      }

      void setHoursMinutes( uint16_t v )
      {
        if( v < 2400 && (v%100) < 60)
        {
          uint8_t b = (uint8_t)((v / 1000) << 4) & 0x30;
          v %= 1000;
          b |= (uint8_t)(v / 100);
          v %= 100;
          buffer[3]= b;

          b= (uint8_t)((v / 10) << 4) & 0x70;
          v%=10;
          b |= (uint8_t)v;
          buffer[2]= b;
          modified= true;
        }
      }

      uint8_t getExtra( uint8_t index )
      {
        if( index < EXTRA_SIZE )
          return buffer[9 + index];
        else
          return 0;
      }

      void setExtra( uint8_t index, uint8_t value )
      {
        if( index < EXTRA_SIZE )
        {
          buffer[9 + index]= value;
          modified= true;
        }
      }

#ifdef DEBUG_DS1307
      void dump()
      {
        out << SABA::hex;
        for(uint8_t i=0;i < 9+EXTRA_SIZE;i++)
          out << buffer[i] << ' ';
        out << SABA::endl;
      }
#endif

    private:

      SABA::I2C::I2CMaster *master;

    protected:

      /** Data has been modified and need to be written by writeDevice() */
      bool modified;
      bool dataAvailable;

    public:
      uint8_t buffer[9 + EXTRA_SIZE];
      DONE_FUNC doneFunc= nullptr;
      void *env;
    };

  }
}

#endif /* DS1307_H_ */