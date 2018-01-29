package de.saba.model;

import java.time.LocalDateTime;

public interface ClockPainter extends ClockStore
{
  enum Ordering
  {
    HourMinuteSecond, HourSecondMinute, MinuteHourSecond, MinuteSecondHour, SecondHourMinute, SecondMinuteHour
  }

  void setOrdering( Ordering ordering );
  Ordering getOrdering();
  
  void paint(LocalDateTime time);
}
