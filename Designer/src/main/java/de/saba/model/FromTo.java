package de.saba.model;

public class FromTo
{
  public final int from;
  public final int to;

  public FromTo(int a, int b)
  {
    super();
    this.from = a;
    this.to = b;
  }

  public int size()
  {
    return 1 + to - from;
  }

  public boolean isEmpty()
  {
    return from == 0 && to == 59;
  }

  public FromTo join(FromTo other)
  {
    if (isEmpty())
      return other;
    if (other.isEmpty())
      return this;

    return new FromTo( Math.max( from, other.from ), Math.min( to, other.to ) );
  }
}
