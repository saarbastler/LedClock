package de.saba.model;

import java.io.PrintWriter;
import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.function.Consumer;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlTransient;

import de.saba.model.ClockHand.Type;

public class ClockFace extends ClockData implements ClockPainter
{
  @XmlElement
  private ClockHand hourHand;

  @XmlElement
  private ClockHand minuteHand;

  @XmlElement
  private ClockHand secondHand;

  @XmlElement
  private Ordering ordering = Ordering.HourMinuteSecond;

  @XmlTransient
  private List<ClockData> children;

  @XmlTransient
  private boolean leds[];

  public ClockFace()
  {
    super( "" );
  }

  public ClockFace(String name)
  {
    super( name );

    hourHand = new ClockHand( Type.Hour);
    minuteHand = new ClockHand( Type.Minute );
    secondHand = new ClockHand( Type.Second );

  }

  @Override
  public List<ClockData> getChildren()
  {
    if (children == null)
    {
      children = new ArrayList<>( 3 );
      children.add( hourHand );
      children.add( minuteHand );
      children.add( secondHand );
    }

    return children;
  }

  @Override
  public void toggleLed(int index)
  {
    // noting to do
  }

  @Override
  public void toggleLedStatus(int index)
  {
    // noting to do
  }

  @Override
  public LedStatus getLedStatus(int index)
  {
    if (leds == null)
      setupLeds( LocalDateTime.now() );

    return leds[index] ? LedStatus.On : LedStatus.Off;
  }

  @Override
  public void paint(LocalDateTime time)
  {
    setupLeds( time );
  }

  @Override
  @XmlTransient
  public Ordering getOrdering()
  {
    return ordering;
  }

  @Override
  public void setOrdering(Ordering ordering)
  {
    this.ordering = ordering;
    leds = null;
  }

  public void setupLeds(LocalDateTime now)
  {
    leds = new boolean[480];

    int hourPos = 5 * (now.getHour() % 12) + (now.getMinute() + 6) / 12;
    switch (ordering)
    {
    case HourMinuteSecond:
      paint( hourHand, hourPos );
      paint( minuteHand, now.getMinute() );
      paint( secondHand, now.getSecond() );
      break;
    case HourSecondMinute:
      paint( hourHand, hourPos );
      paint( secondHand, now.getSecond() );
      paint( minuteHand, now.getMinute() );
      break;
    case MinuteHourSecond:
      paint( minuteHand, now.getMinute() );
      paint( hourHand, hourPos );
      paint( secondHand, now.getSecond() );
      break;
    case MinuteSecondHour:
      paint( minuteHand, now.getMinute() );
      paint( secondHand, now.getSecond() );
      paint( hourHand, hourPos );
      break;
    case SecondHourMinute:
      paint( secondHand, now.getSecond() );
      paint( hourHand, hourPos );
      paint( minuteHand, now.getMinute() );
      break;
    case SecondMinuteHour:
      paint( secondHand, now.getSecond() );
      paint( minuteHand, now.getMinute() );
      paint( hourHand, hourPos );
      break;
    }
  }

  @SuppressWarnings("incomplete-switch")
  private void paint(ClockStore clockPainter, int offset)
  {
    for (int i = 0; i < 60; i++)
    {
      for (int n = 0; n < 8; n++)
      {
        LedStatus ledStatus = clockPainter.getLedStatus( i + n * 60 );

        int dest = ((i + offset) % 60) + n * 60;

        switch (ledStatus)
        {
        case On:
          leds[dest] = true;
          break;
        case Mask:
          leds[dest] = false;
          break;
        }
      }
    }
  }

  private void ordered(Consumer<ClockHand> consumer)
  {
    switch (ordering)
    {
    case HourMinuteSecond:
      consumer.accept( hourHand );
      consumer.accept( minuteHand );
      consumer.accept( secondHand );
      break;
    case HourSecondMinute:
      consumer.accept( hourHand );
      consumer.accept( secondHand );
      consumer.accept( minuteHand );
      break;
    case MinuteHourSecond:
      consumer.accept( minuteHand );
      consumer.accept( hourHand );
      consumer.accept( secondHand );
      break;
    case MinuteSecondHour:
      consumer.accept( minuteHand );
      consumer.accept( secondHand );
      consumer.accept( hourHand );
      break;
    case SecondHourMinute:
      consumer.accept( secondHand );
      consumer.accept( hourHand );
      consumer.accept( minuteHand );
      break;
    case SecondMinuteHour:
      consumer.accept( secondHand );
      consumer.accept( minuteHand );
      consumer.accept( hourHand );
      break;
    }

  }

  public void export(PrintWriter out, AtomicInteger index)
  {
    ordered( ch -> ch.export(out,index.getAndIncrement()));
  }

  public void exportStruct(PrintWriter out, AtomicInteger index)
  {
    ordered( ch -> ch.exportStruct(out,index.getAndIncrement()));
  }

}
