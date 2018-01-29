package de.saba.model;

import java.util.ArrayList;
import java.util.List;

import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class Clock extends ClockData
{
  @XmlElement
  private ClockFaces clockFaces = new ClockFaces( "Zeiger" );
  
  @XmlElement
  private ClockDials clockDials= new ClockDials( "Ziffernblätter" );
  
  public Clock()
  {
    super("");
  }
  
  public Clock(String name)
  {
    super( name );
  }

  @Override
  public List<ClockData> getChildren()
  {
    ArrayList<ClockData> list= new ArrayList<>(2);
    list.add( clockFaces );
    list.add( clockDials );
    
    return list;
  }

  public ClockFaces getClockFaces()
  {
    return clockFaces;        
  }
  
  public ClockDials getClockDials()
  {
    return clockDials;        
  }
}
