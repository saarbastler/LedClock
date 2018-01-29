package de.saba.test;

import org.junit.Test;

import de.saba.model.ClockUtils;
import de.saba.model.FromTo;

import static org.junit.jupiter.api.Assertions.*;

public class TestClockUtils
{
  @Test
  public void biggestGap()
  {
    byte data[] = new byte[60];
    data[0] = 1;
    data[29] = 1;

    FromTo gap = ClockUtils.findBiggestGap( data );

    assertNotNull( gap );
    assertEquals( gap.from, 30 );
    assertEquals( gap.to, 59 );
    assertEquals( gap.size(), 30 );
    assertEquals( false, gap.isEmpty() );

    data[40] = 1;
    gap = ClockUtils.findBiggestGap( data );

    assertNotNull( gap );
    assertEquals( gap.from, 1 );
    assertEquals( gap.to, 28 );
    assertEquals( gap.size(), 28 );
    assertEquals( false, gap.isEmpty() );
  }

  @Test
  public void noGap()
  {
    byte data[] = new byte[60];

    FromTo gap = ClockUtils.findBiggestGap( data );
    assertNotNull( gap );
    assertEquals( gap.from, 0 );
    assertEquals( gap.to, 59 );
    assertEquals( 60, gap.size() );
    assertEquals( true, gap.isEmpty() );
  }

  @Test
  public void join()
  {
    FromTo a = new FromTo( 10, 50 );
    FromTo b = new FromTo( 9, 49 );

    FromTo join = a.join( b );
    assertNotNull( join );
    assertEquals( 10, join.from );
    assertEquals( 49, join.to );
    assertEquals( join.size(), 40 );
    assertEquals( join.isEmpty(), false );

  }
}
