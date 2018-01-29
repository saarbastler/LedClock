package de.saba.model;

import java.util.List;

import javax.xml.bind.annotation.XmlElement;

public abstract class ClockData
{
  @XmlElement
  private String name;

  public abstract List<? extends ClockData> getChildren();
  
  public ClockData(String name)
  {
    super();
    this.name = name;
  }
  
  public String getName()
  {
    return name;
  }

  @Override
  public String toString()
  {
    return name;
  }
  
  
}
