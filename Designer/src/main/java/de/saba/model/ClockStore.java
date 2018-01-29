package de.saba.model;

public interface ClockStore
{
  enum LedStatus
  {
    Off, On, Mask
  };
  
  void toggleLed(int index);
  
  void toggleLedStatus( int index );
  
  LedStatus getLedStatus(int index);  
}
