package de.saba;

import java.time.LocalDateTime;

import de.saba.model.ClockPainter;
import de.saba.model.ClockStore;
import de.saba.model.ClockStore.LedStatus;
import javafx.event.EventHandler;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;

public class ClockPane extends Canvas implements Constants
{
  private int lastSelected = -1;

  private ClockStore clockStore;

  public ClockPane()
  {
    super( 2 * (int) offsetX, 2 * (int) offsetY );

    addEventHandler( MouseEvent.MOUSE_MOVED, new EventHandler<MouseEvent>()
    {
      @Override
      public void handle(MouseEvent event)
      {
        if (clockStore != null)
        {
          int selected = ledIndex( event.getX() - offsetX, event.getY() - offsetY );
          if (selected != lastSelected)
          {
            GraphicsContext gc = getGraphicsContext2D();

            if (lastSelected >= 0)
            {
              Led led = ledCoordinate( lastSelected );

              led.paintLed( gc, clockStore.getLedStatus( lastSelected ), false );
            }
            lastSelected = selected;
            if (lastSelected >= 0)
            {
              Led led = ledCoordinate( lastSelected );

              led.paintLed( gc, clockStore.getLedStatus( lastSelected ), true );
            }
          }
        }
      }
    } );
    addEventHandler( MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>()
    {
      @Override
      public void handle(MouseEvent event)
      {
        if (clockStore != null)
        {
          int index = ledIndex( event.getX() - offsetX, event.getY() - offsetY );
          if (index >= 0 && index < 480)
          {
            MouseButton button = event.getButton();
            if (button.equals( MouseButton.PRIMARY ))
              clockStore.toggleLed( index );
            else if (button.equals( MouseButton.SECONDARY ))
              clockStore.toggleLedStatus( index );

            Led led = ledCoordinate( lastSelected );

            GraphicsContext gc = getGraphicsContext2D();
            led.paintLed( gc, clockStore.getLedStatus( lastSelected ), true );
          }
        }
      }
    } );

    paint();
  }

  public void setClockPainter(ClockStore value)
  {
    clockStore = value;
    if (clockStore != null && clockStore instanceof ClockPainter)
      ((ClockPainter) clockStore).paint( LocalDateTime.now() );

    paint();
  }

  protected int ledIndex(double x, double y)
  {
    double alpha = Math.atan2( y, x ) * 180.0 / Math.PI;
    double r = Math.sqrt( x * x + y * y );

    double dRing = (r + Led.ledRadius / 2 - radiusMin) / radiusStep;
    int ring = (int) (dRing + .5);
    if (ring >= 0 && ring < 8)
    {
      double a = alpha + 93.0;
      if (a < 0.0)
        a += 360.0;

      int index = (int) (a / 6.0);

      int led = ring * 60 + index;

      return led;
    }

    return -1;
  }

  protected Led ledCoordinate(int index)
  {
    if (index < 0 || index >= 480)
      throw new IllegalArgumentException( "Index out of range: " + index );

    int ring = index / 60;
    int i = index % 60;

    double radius = radiusMin + ring * radiusStep;
    double alpha = (i * 6.0 - 90.0) * Math.PI / 180.0;

    return new Led( offsetX + radius * Math.cos( alpha ), offsetY + radius * Math.sin( alpha ) );
  }

  public void paint()
  {
    GraphicsContext gc = getGraphicsContext2D();

    gc.setFill( Color.WHITE );
    gc.clearRect( 0, 0, getWidth(), getHeight() );
    gc.setStroke( Color.BLACK );

    for (int index = 0; index < 480; index++)
    {
      Led coord = ledCoordinate( index );

      LedStatus ledStatus = clockStore == null ? LedStatus.Off : clockStore.getLedStatus( index );

      coord.paintLed( gc, ledStatus, false );
    }
  }

  public void repaint(LocalDateTime time)
  {
    if (clockStore != null && clockStore instanceof ClockPainter)
    {
      if (time == null)
        time = LocalDateTime.now();

      ((ClockPainter) clockStore).paint( time );
      paint();
    }
  }
}
