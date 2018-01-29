package de.saba.model;

import java.io.PrintWriter;
import java.util.Collections;
import java.util.List;

import javax.xml.bind.annotation.XmlElement;

public class ClockDial extends ClockData implements ClockStore
{
  @XmlElement
  private byte[] data = new byte[60];

  public ClockDial()
  {
    this( "" );
  }

  public ClockDial(String name)
  {
    super( name );
  }

  @Override
  public List<? extends ClockData> getChildren()
  {
    return Collections.emptyList();
  }

  @Override
  public void toggleLed(int index)
  {
    data[index % 60] ^= (1 << (index / 60));
  }

  @Override
  public void toggleLedStatus(int index)
  {
    // nothing to do
  }

  @Override
  public LedStatus getLedStatus(int index)
  {
    if ((data[index % 60] & (1 << (index / 60))) != 0)
      return LedStatus.On;
    else
      return LedStatus.Off;
  }

  public void export(PrintWriter out, int index)
  {
    int columns = 20;
    for (int i = 0; i < data.length; i++)
    {
      if (i + index > 0)
        out.print( ", " );

      if (columns-- == 0)
      {
        out.println();
        out.print( "  " );
        columns = 19;
      }

      out.format( "0x%02x", data[i] );
    }
    out.println();
  }

}
