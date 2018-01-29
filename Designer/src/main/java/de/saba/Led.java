package de.saba;

import de.saba.model.ClockStore;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Led implements Constants 
{
  private double x;
  private double y;

  public Led(double x, double y)
  {
    super();
    this.x = x;
    this.y = y;
  }

  public Led()
  {
    this(0.0, 0.0);
  }

  public double getX()
  {
    return x;
  }

  public double getY()
  {
    return y;
  }

  public void paintLed(GraphicsContext gc, ClockStore.LedStatus ledStatus, boolean selected)
  {
    switch(ledStatus)
    {
    case Off:
      gc.setFill(Color.WHITE);
      break;
      
    case On:
      gc.setFill( Color.RED );
      break;
      
    case Mask:
      gc.setFill( Color.YELLOW );
      break;
      
    }
    gc.setStroke(selected ? Color.BLUE : Color.BLACK);
    
    gc.fillOval(x - ledRadius, y - ledRadius, 2 * ledRadius, 2 * ledRadius);      
    gc.strokeOval(x - ledRadius, y - ledRadius, 2 * ledRadius, 2 * ledRadius);      
  }
}
