package de.saba.model;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlElement;

public class ClockFaces extends ClockData
{
  @XmlElement
  private List<ClockFace> clockFaces= new ArrayList<>();
  
  public ClockFaces()
  {
    super("");
  }
  
  public ClockFaces(String name)
  {
    super( name );
  }

  @Override
  public List<ClockFace> getChildren()
  {
    return clockFaces;
  }
}
