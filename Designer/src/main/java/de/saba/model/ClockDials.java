package de.saba.model;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlElement;

public class ClockDials extends ClockData
{
  @XmlElement
  private List<ClockDial> children = new ArrayList<>();

  public ClockDials()
  {
    super( "" );
  }

  public ClockDials(String name)
  {
    super( name );
  }

  @Override
  public List<ClockDial> getChildren()
  {
    return children;
  }
}
