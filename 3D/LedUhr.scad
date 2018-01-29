include <batteriefach.scad>

$fn=100;
breite= 210;
dicke= 2;

module alpsEC11Pin()
{
  translate([-.3/2,-.9/2,-4.5]) cube([0.3, 0.9, 4.5]);
  
  translate([-.3/2,-1.5/2,0]) cube([0.3, 1.5, 3]);
  
  translate([-1.5+.3/2,-1.5/2,3]) cube([1.5, 1.5, .3]);
}
// 
module alpsEC11()
{
  translate([-13.6/2,-11.7/2,0])
  {
    color("green") cube([13.6,11.7,3.5]);
    
    color("silver") translate([0,0,3.5]) cube([13.6,11.7,2.5]);
  }
  
  color("silver")
  {
    translate([0,0,6]) cylinder(d=7,h=4.5);
    
    translate([0,0,10.5]) cylinder(d=4,h=0.7);
    
    translate([0,0,11.2]) difference()
    {
      cylinder(d=6,h=6.6);
      
      translate([-.5, -4, 6.6-3.5]) cube([1, 8, 5]);
    }
    
    translate([ 7.5,-2.5,0]) alpsEC11Pin();
    translate([ 7.5,   0,0]) alpsEC11Pin();
    translate([ 7.5, 2.5,0]) alpsEC11Pin();
    
    translate([-7.5,-2.5,0]) rotate([0,0,180]) alpsEC11Pin();
    translate([-7.5, 2.5,0]) rotate([0,0,180]) alpsEC11Pin();
  }
}

module leduhr()
{
  difference()
  {
    translate([-202/2,-202/2,0]) cube([202,202,1.5]);
    
    translate([-190/2,-202/2+12,-1]) cylinder(d=4.2,h=3);
    translate([-190/2,-202/2+12+24.5,-1]) cylinder(d=4.2,h=3);
    
    translate([ 190/2,-202/2+12,-1]) cylinder(d=4.2,h=3);
    translate([ 190/2,-202/2+12+24.5,-1]) cylinder(d=4.2,h=3);
  }
  
  color("red") translate([0,0,1.5]) difference()
  {
    translate([0,0,20]) cylinder(d=198,h=8);
    
    translate([0,0,-1]) cylinder(d=105,h=30);
  }
  
  color("darkgrey") translate([-6/2,-25/2,-19]) cube([6,25,19]);
}

module platine()
{
  translate([-62/2,10,2]) cube([62,52, 1.5]);
}

module frontscheibe()
{
  %translate([-breite/2,-breite/2-8,31]) cube([breite,breite+8,dicke]);
  
  echo("Frontscheibe:", breite, breite+8, dicke);
}

alpha= 15;
module deckel()
{
 %translate([-breite/2,21.5,209.5-dicke]) cube([breite,54,dicke]);
 echo("Deckel: ", breite, 54, dicke);
}

module rueckseite()
{
  %translate([-breite/2,74,-3.5]) cube([breite,dicke,211]);
  echo("Rückseite: ", breite,211,dicke);
}

module boden()
{
  %translate([-breite/2, -35,-5.5]) cube([breite,111,dicke]);
  echo("Boden",breite,111,dicke);
}

module seite()
{
  %rotate([0,-90,0]) linear_extrude(height=dicke)
    polygon(points=[[-3.5,-33],[-3.5,74],[207.5,74],[207.5,23.5]]);
    
  echo("Seite: ", 211, 107, 50, dicke);
}

module gehaeuse()
{
  rotate([-alpha,0,0]) translate([0,0,202/2]) rotate([90,0,0])
  {
    leduhr();

    frontscheibe();
  }
  deckel();
  rueckseite();
  boden();

  translate([breite/2,0,0]) seite();
  translate([-breite/2+dicke,0,0]) seite();
}

module staender1()
{
  translate([-16, 0, -3.5]) 
  {
    difference()
    {
      union()
      {
        difference()
        {
          cube([14, 20, 50]);
          
          translate([0,-11,0]) rotate([-alpha,0,0]) translate([-1,-5,0]) cube([16,15,60]);
        }
        
        translate([0,20,0]) cube([14,54,15]);
        
        difference()
        {
          translate([0,-34,0]) union()
          {
            cube([14,35,3]);
            cube([14,20,15]);
          }
          translate([0,-43.5,0]) rotate([-alpha,0,0]) translate([-1,-5,0]) cube([16,15,30]);
        }
      }    

      translate([16.5,111/2-35, 7.5]) rotate([0,-90,0]) cylinder(d=2.5, h=15);
      translate([7,76, 7.5]) rotate([90,0,0]) cylinder(d=2.5, h=13);
      translate([7,60, -2]) rotate([0,0,0]) cylinder(d=2.5, h=15);
      translate([7,-18, -2]) rotate([0,0,0]) cylinder(d=2.5, h=15);
      translate([7,-33, 8]) rotate([-alpha-90,0,0]) cylinder(d=2.5, h=13);
      
      rotate([-alpha,0,0]) translate([0,0,0]) rotate([90,0,0])
      {
        translate([ 6,+15.4,-8]) cylinder(d=3.2,h=13);
        translate([ 6,+15.5+24.5,-8]) cylinder(d=3.2,h=13);
      }
      
    }
    *translate([16.5,111/2-35, 7.5]) rotate([0,-90,0]) M3SK(10);
    
    *translate([7,76, 7.5]) rotate([90,0,0]) M3SK(10);
    *translate([7,60, -2]) rotate([0,0,0]) M3SK(10);
    
    *translate([7,-18, -2]) rotate([0,0,0]) M3SK(10);
    *translate([7,-33, 8]) rotate([-alpha-90,0,0]) M3SK(10);
  }
}

module staender2()
{
  translate([breite/2-13-3,20,197.5]) difference()
  {
    cube([14,53.75,10]);
  
    translate([-1,-9.5,0]) rotate([-alpha,0,0]) cube([16, 10, 20]);
    
    translate([7,40,5]) rotate([-90,0,0]) cylinder(d=2.5, h=15);
    
    translate([7,0,6.5]) rotate([-alpha-90,0,0]) cylinder(d=2.5, h=13);
    
    translate([7,53.75/2,3]) cylinder(d=2.5,h=10);
    
    translate([4,15,5]) rotate([0,90,0]) cylinder(d=2.5,h=20);
    translate([4,15+22,5]) rotate([0,90,0]) cylinder(d=2.5,h=20);
  }
}

module bohrschablone1()
{
  clearance= .5;
  
  difference()
  {
    translate([breite/2-14,-38,-7.5]) cube([17,117,16]);
    
    translate([breite/2-9,60, -10]) rotate([0,0,0]) cylinder(d=3, h=15);
    translate([breite/2-9,-18, -10]) rotate([0,0,0]) cylinder(d=3, h=15);
      
    translate([breite/2-16,0,-3.5])
    {
      translate([16.5+5,111/2-35, 7.5]) rotate([0,-90,0]) cylinder(d=3, h=15);
      translate([7,76+5, 7.5]) rotate([90,0,0]) cylinder(d=3, h=13);
      translate([7,-33, 8]) rotate([-alpha-90,0,0]) translate([0,0,-10]) cylinder(d=3, h=13);
    } 
    translate([breite/2-20,-35-clearance+5,-5.5]) cube([20+clearance,111-5+2*clearance,18]);
    translate([breite/2-20,-35-clearance,-5.5]) cube([20+clearance,10,2]);
    
    translate([breite/2-20,-35-clearance,-3.5]) rotate([-alpha,0,0]) cube([20+clearance,5,20]);
  }
}

module encoder()
{
  module header(pins, rows)
  {
    color("darkgrey") cube([2.54*pins,2.54*rows,1.27]);
    
    for(x=[0:pins-1],y=[0:rows-1])
      translate([x*2.54+(1.27+.6)/2,y*2.54+(1.27+.6)/2,-3.5]) cube([0.6,0.6,11.5]);
  }

  translate([breite/2-30,0,0]) rotate([90-alpha,0,0]) translate([0,2,25]) 
  {
    rotate([0,0,90]) alpsEC11();
  
    translate([-11,-10,-1.5]) cube([22,24,1.5]);
    
    //translate([-8.5,-7,2]) rotate([180,0,0]) M2(10);
    //translate([ 8.5, 7,2]) rotate([180,0,0]) M2(10);
    
    translate([-5,11,-1.5]) rotate([180,0,0]) header(4,1);
  }
}

module encoderHalter()
{
  translate([breite/2-43,0,0]) rotate([90-alpha,0,0]) translate([0,-10,1.75]) difference()
  {
    cube([26.5,25,21.5]);
    
    translate([26.5/2,2+25/2,-3]) cylinder(d=2.5,h=30);
    
    translate([4.5,5,12]) cylinder(d=1.5,h=20);
    translate([4.5+17,5+14,12]) cylinder(d=1.5,h=20);
    
    translate([8,19,-1]) cylinder(d=4,h=25);
    translate([6,19,-1]) cube([13.5,8,25]);
    translate([8,17,-1]) cube([9.5,8,25]);
    translate([17.5,19,-1]) cylinder(d=4,h=25);

    translate([8,4,-1]) cylinder(d=4,h=25);
    translate([6,-4,-1]) cube([13.5,8,25]);
    translate([8,-2,-1]) cube([9.5,8,25]);
    translate([17.5,4,-1]) cylinder(d=4,h=25);
    
    rotate([alpha+90,0,0]) translate([-1,-2,-6]) cube([30,30,10]);
  }
}

translate([60,70,10]) rotate([90,0,0]) batteriefach();
encoderHalter();
encoder();

*bohrschablone1();
*translate([0,39,0]) rotate([0,0,180]) bohrschablone1();

translate([breite/2,0,0]) staender1();
translate([-breite/2,0,0]) mirror([1,0,0])staender1();
staender2();
mirror([1,0,0]) staender2();

gehaeuse();
platine();

// Druck
*rotate([0,-90,0])staender1();
*rotate([0,90,0])mirror([1,0,0])staender1();

*staender2();
*mirror([1,0,0]) staender2();

*bohrschablone1();
*mirror([1,0,0]) bohrschablone1();

*rotate([alpha+90,0,0]) encoderHalter();
