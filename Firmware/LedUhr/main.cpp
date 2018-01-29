#include "io_config.h"

#include <saba_encoder.h>

#include "LedUhr.h"
#include "Sprite.h"

#include "DS1307.h"
#include "MenuDef.h"
#include "eeprom.h"

// Timing, ~100 Hz
//SABA::Timer1 timer1;

// triggers 5 times a second
SABA::Timing::SingleDelayFixed<uint16_t,400> timeDelay;

SABA::I2C::Master0 i2cMaster;
// index of dial shown in Display
uint8_t dialIndex;
// index of face shown in Display
uint8_t faceIndex;

// The mode bit vector
uint8_t mode;

uint8_t counter;

SABA::I2C::DS1307<0> ds1307(static_cast<SABA::I2C::I2CMaster*>(&i2cMaster));

SABA::TurnEncoder<(SABA::SFRA)&PIND,PHASE1_BIT,(SABA::SFRA)&PIND,PHASE2_BIT,(SABA::SFRA)&PIND,BUTTON_BIT,1> encoder;

void paintPropeller(uint8_t offset)
{
  LedUhr::clear();
  for(uint8_t i=0;i < 60; i++)
    LedUhr::led( i & 7, (i + offset) % 60, true);

  LedUhr::toggleRam();
}

void initializeApplication()
{
  OSCCAL= 0x90;
  LedUhr::init();

  dialIndex= 0;
  faceIndex= 0;
  mode= SABA::BIT(MODE_GREETING);
  counter= 59;
  currentMenu= (Menu*)pgm_read_ptr(&menus[0]);
  
  paintPropeller(0);

  i2cMaster.baudrate( 60, SABA::I2C::By1) // = 100kHz
    .enable(true);

  SABA::Timer1 timer1;
  timer1.clockSelect(SABA::Timer16::By64)
    .waveformGenerationMode(SABA::Timer16::FAST_PWM_ICR1)
    .enableOverflowInterrupt(true)
    .icr= 62;

  readConfiguration();
  
  sei();
  timeDelay.start();

#ifdef DEBUG
  out << PSTR("LedUhr") << SABA::endl;
#endif // DEBUG
}

void paintDateTime()
{
  drawDial(dialIndex);
  drawTime(faceIndex, ds1307.getHours(), ds1307.getMinutes(), ds1307.getSeconds());

  uint8_t seconds= ds1307.getSeconds();
  if( (seconds / 10) & 1 )
  {
    uint16_t dm= ds1307.getDate() * 100 + ds1307.getMonth();
    LedUhr::digitNumber( dm );
    LedUhr::decimalPoint(0, true);
    LedUhr::decimalPoint(2, true);
  }
  else
  {
    LedUhr::digitNumber( ds1307.getHoursMinutes());
    LedUhr::decimalPoint(2, seconds & 1);
  }
}

void setMenu()
{
  LedUhr::clear();
  currentMenu= (Menu*)pgm_read_ptr(&menus[menuIndex]);
  currentMenu->paintMenu(0);

  LedUhr::toggleRam();  
}

void cyclic()
{
  i2cMaster.statemachine();

  encoder.cyclic([](int8_t counter, int8_t increment )
  {
    //out << (uint8_t)counter << ' ' << (uint8_t)increment << SABA::endl;
    if( mode & SABA::BIT(MODE_MENU_ACTIVE))
    {
      LedUhr::clear();
      currentMenu->paintMenu(0);
      currentMenu->turn(counter, increment);
      LedUhr::toggleRam();
      
      menuTimeout= 0;
    }
    else if( mode & SABA::BIT(MODE_MENU))
    {
      //mode &= ~(SABA::BIT(MODE_MENU)|SABA::BIT(MODE_MENU_ACTIVE));
      if(increment < 0)
      {
        --menuIndex;
        if(menuIndex >= menuMax())
          menuIndex= menuMax()-1;
      }
      else
      {
        ++menuIndex;
        if(menuIndex >= menuMax())
          menuIndex= 0;
      }

      setMenu();
      menuTimeout= 0;
    }      
  },[](bool down)
  {
    //out << down << SABA::endl;
    if( down )
    {
      if( mode & SABA::BIT(MODE_MENU_ACTIVE))
      {
        LedUhr::clear();
        currentMenu->paintMenu(0);
        currentMenu->click(down);
        LedUhr::toggleRam();
      }
      else if( mode & SABA::BIT(MODE_MENU))
      {
        mode |= SABA::BIT(MODE_MENU_ACTIVE);
        
        LedUhr::clear();
        currentMenu->activate();
        LedUhr::toggleRam();
      }
      else
      {
        menuIndex= 0;
        menuTimeout= 0;
        mode= (mode & ~(SABA::BIT(MODE_GREETING)|SABA::BIT(MODE_MENU_ACTIVE))) | SABA::BIT(MODE_MENU);
        
        setMenu();
      }
    }
  });

  
  if( ds1307.isModified())
    ds1307.writeDevice();

  if(timeDelay())
  {
    ds1307.readDevice([](void *env, bool success)
    {
      if(success)
      {
        if( !ds1307.isClockEnabled())
        {
          mode |= SABA::BIT(MODE_DS1307_BATTERY_EMPTY);
          ds1307.enableClock(true);
        }

        if( mode & SABA::BIT(MODE_GREETING))
        {
          if( --counter > 59)
            mode &= ~SABA::BIT(MODE_GREETING);

          paintPropeller(counter);
        }
        else if((mode & SABA::BIT(MODE_MENU)) == 0)
        {
          paintDateTime();
          LedUhr::toggleRam();
        }
      }
    });

    if( mode && SABA::BIT(MODE_MENU))
    {
      ++menuTimeout;
      if( menuTimeout > 90)
        mode &= ~(SABA::BIT(MODE_MENU)|SABA::BIT(MODE_MENU_ACTIVE));
    }
    
    timeDelay.start();
  }

  //if(usart.readyToSend())
  //  usart.putch(0x55);
}

#ifdef DEBUG
void dumpI2C(const SABA::I2C::CMD *cmd)
{
  uint8_t i;
  SABA::SFREG<(SABA::SFRA)&TWSR> twsr;

  out << PSTR("a:") << cmd->address << PSTR(" E:") << cmd->error << PSTR(" TWSR:") << twsr() << SABA::endl;
  out << 'W' << '[' << cmd->bytesToWrite << ']';
  for(i=0; i < cmd->bytesToWrite;i++)
    out << cmd->writeBuffer[i] << ' ';

  out << SABA::endl << 'R' << '[' << cmd->bytesToRead << ']';
  for(i=0; i < cmd->bytesToRead;i++)
    out << cmd->readBuffer[i] << ' ';

  out << SABA::endl;
}

bool applicationCommand(char cmd)
{
  uint8_t a, b;
  switch(cmd)
  {
#ifdef DEBUG_DS1307
    case 'D':
      ds1307.dump();
      return true;
#endif

    /*case 'r':
      out << SABA::hex;
      for(uint8_t i=0;i < 8;i++)
      {
        for(uint8_t n=0;n < 8;n++)
          out << LedUhr::getRam(i,n) << ' ';
        out << SABA::endl;
      }
      return true;*/

    case 'l':
      a= cmdline.nextHex<uint8_t>();
      if(cmdline())
      {
        b= cmdline.nextDec<uint8_t>();
        if( cmdline())
        {
          char c= cmdline.nextCharIgnoreBlank();

          LedUhr::led(a, b, c != '0');
          return true;
        }
      }
      break;

    /*case '1':
      for(uint8_t i=0;i < 8;i++)
        for(uint8_t n=0;n < 8;n++)
          LedUhr::setRam(i,n,0xff);
      return true;*/

    case '0':
      LedUhr::clear();
      return true;

    case 't':
      out << (ds1307.isDataAvailable() ? '+' : '-') << (ds1307.isModified() ? '*' : '.')
       << SABA::dec << ds1307.getHours() << ':' << ds1307.getMinutes() << ':' << ds1307.getSeconds() << SABA::endl;
       LedUhr::digitNumber( ds1307.getHoursMinutes());
      return true;

    case 'd':
      out << (ds1307.isDataAvailable() ? '+' : '-') << (ds1307.isModified() ? '*' : '.')
      << SABA::dec << ds1307.getDate() << ':' << ds1307.getMonth() << ':' << ds1307.getYear() << SABA::endl;
      LedUhr::digitNumber( ds1307.getHoursMinutes());
      return true;

    case 'T':
      a= cmdline.nextDec<uint8_t>();
      if( cmdline() )
      {
        mode &= ~SABA::BIT(MODE_DS1307_BATTERY_EMPTY);
        ds1307.setHours(a);
        a= cmdline.nextDec<uint8_t>();
        if( cmdline() )
        {
          ds1307.setMinutes(a);
          a= cmdline.nextDec<uint8_t>();
        }
        if( cmdline() )
          ds1307.setSeconds(a);
      }
      return true;

    case 'D':
      a= cmdline.nextDec<uint8_t>();
      if( cmdline() )
      {
        mode &= ~SABA::BIT(MODE_DS1307_BATTERY_EMPTY);
        ds1307.setDate(a);
        a= cmdline.nextDec<uint8_t>();
        if( cmdline() )
        {
          ds1307.setMonth(a);
          a= cmdline.nextDec<uint8_t>();
        }
        if( cmdline() )
          ds1307.setYear(a);
      }
      return true;

    case 'f':
      a= cmdline.nextDec<uint8_t>();
      if( cmdline() && a < FACES)
      {
        faceIndex= a;
        return true;        
      }
      break;

    case 'F':
      a= cmdline.nextDec<uint8_t>();
      if( cmdline() && a < DIALS)
      {
        dialIndex= a;
        return true;
      }
      break;

    case 'b':
      a= cmdline.nextHex<uint8_t>();
      if( cmdline() )
      {
        SABA::Timer0 timer0;
        timer0.ocra= a;
        return true;
      }      
  }
  return false;
}
#endif // DEBUG
