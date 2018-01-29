package de.saba.model;

import java.io.PrintWriter;
import java.util.Collections;
import java.util.List;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlTransient;

public class ClockHand extends ClockData implements ClockStore
{
  enum Type
  {
    Hour, Minute, Second
  }

  private Type type;

  private int offset;

  private int width;

  private byte data[];

  @XmlTransient
  private boolean ledOn[];

  @XmlTransient
  private boolean ledMask[];

  public ClockHand()
  {
    super( "" );
  }

  public ClockHand(Type type)
  {
    super( type.toString() );

    this.type = type;
  }

  @Override
  public List<ClockData> getChildren()
  {
    return Collections.emptyList();
  }

  public void toggleLed(int index)
  {
    if (ledOn == null)
      setupLeds();

    data = null;
    ledOn[index] = !ledOn[index];
  }

  public boolean[] getLedOn()
  {
    if (ledOn == null)
      setupLeds();

    return ledOn;
  }

  @Override
  public void toggleLedStatus(int index)
  {
    if (ledMask == null)
      setupLeds();

    data = null;
    ledMask[index] = !ledMask[index];
  }

  @Override
  public LedStatus getLedStatus(int index)
  {
    if (ledOn == null || ledMask == null)
      setupLeds();

    if (ledOn[index])
      return LedStatus.On;
    else
      return ledMask[index] ? LedStatus.Mask : LedStatus.Off;
  }

  @XmlElement
  public int getOffset()
  {
    if (data == null)
      setupData();

    return offset;
  }

  public void setOffset(int offset)
  {
    this.offset = offset;
    ledOn = null;
    ledMask = null;
  }

  @XmlElement
  public int getWidth()
  {
    if (data == null)
      setupData();

    return width;
  }

  public void setWidth(int width)
  {
    this.width = width;
    ledOn = null;
    ledMask = null;
  }

  @XmlElement
  public Type getType()
  {
    if (type == null)
      switch (getName())
      {
      case "Hour":
        type = Type.Hour;
        break;
      case "Minute":
        type = Type.Minute;
        break;
      case "Second":
        type = Type.Second;
        break;
      }
    return type;
  }

  @XmlElement
  public byte[] getData()
  {
    if (data == null)
      setupData();

    return data;
  }

  public void setData(byte[] data)
  {
    this.data = data;
    ledOn = null;
    ledMask = null;
  }

  private void setupData()
  {
    byte on[] = new byte[60];
    byte mask[] = new byte[60];

    for (int i = 0; i < 60; i++)
    {
      on[i] = ClockUtils.bitvector( ledOn, i );
      mask[i] = ClockUtils.bitvector( ledMask, i );
    }

    FromTo onGap = ClockUtils.findBiggestGap( on );
    FromTo maskGap = ClockUtils.findBiggestGap( mask );
    FromTo join = onGap.join( maskGap );

    if (join.isEmpty())
    {
      offset = 0;
      width = 0;
      data = null;
    }
    else
    {
      offset = join.to - 59;
      width = 60 - join.size();

      data = new byte[2 * width];

      int index = 0;
      for (int i = 0; i < width; i++)
      {
        data[index++] = on[(1 + join.to + i) % 60];
        data[index++] = mask[(1 + join.to + i) % 60];
      }
    }
  }

  protected void setupLeds()
  {
    ledOn = new boolean[480];
    ledMask = new boolean[480];

    if (data != null && width > 0)
    {
      int index = (offset < 0) ? 60 + offset : offset;
      for (int i = 0; i < width; i++)
      {
        ClockUtils.reverseBitvector( data[2 * i], index, ledOn );
        ClockUtils.reverseBitvector( data[2 * i + 1], index, ledMask );

        ++index;
        if (index >= 60)
          index -= 60;
      }
    }
  }

  public void export(PrintWriter out, int counter)
  {
    getData();

    out.print( "const uint8_t spritedata" );
    out.print( counter );
    out.print( "[] PROGMEM = { " );

    int columns = 8;
    for (int i = 0; i < data.length; i++)
    {
      if (i > 0)
        out.print( ", " );

      if (columns-- == 0)
      {
        out.println();
        out.print( "  " );
        columns = 16;
      }

      out.format( "0x%02x", data[i] );

    }

    out.println( " };" );

  }

  public void exportStruct(PrintWriter out, int index)
  {
    if (index > 0)
    {
      out.print( "," );
      if ((index % 3) == 0)
      {
        out.println();
        out.print( "  " );
      }
    }
    out.print( "{" );

    out.print( offset );
    out.print( "," );
    out.print( width );
    out.print( "," );
    out.print( getType().ordinal() );
    out.print( "," );
    out.print( "spritedata" );
    out.print( index );

    out.print( "}" );

  }

}
