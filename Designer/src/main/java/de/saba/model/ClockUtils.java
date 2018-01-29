package de.saba.model;

public class ClockUtils
{

  public static FromTo findBiggestGap(byte data[])
  {
    int i = 0;
    FromTo fromTo = null;
  
    while (i < 60)
    {
      for (; i < 60; i++)
        if (data[i] == 0)
          break;
  
      int from = i;
      for (++i; i < 60; i++)
        if (data[i] != 0)
          break;
  
      if( fromTo == null || fromTo.size() < (1+i-from))
        fromTo= new FromTo(from, i-1);
    }
    
    return fromTo;
  }

  public static byte bitvector(boolean data[], int offset)
  {
    byte result = 0;
    for (int i = 0; i < 8; i++)
      if (data[offset + i * 60])
        result |= 1 << i;
  
    return result;
  }

  public static void reverseBitvector(byte b, int index, boolean data[])
  {
    for (int i = 0; i < 8; i++,b >>= 1)
      if( (b & 1) != 0)
        data[index + i*60]= true;
  }
}
