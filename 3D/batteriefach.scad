$fn=100;

module aaa()
{
  color("aqua")
  {
    cylinder(d=10.5,h=42);
    translate([0,0,42]) cylinder(d=5.5,h=1);
    translate([0,0,42]) cylinder(d=3,h=2);
  }
}

module kontakt()
{
  color("silver")
  {
    translate([-9.3/2,-9.8/2,0]) cube([9.3,9.8,0.3]);
    cylinder(d=4, h=1.6);

    translate([-2.8/2, 9.8/2,0]) cube([2.8,6,0.3]);
  }
}

module feder()
{
  color("silver")
  {
    translate([-9.3/2,-9.8/2,0]) cube([9.3,9.8,0.3]);
    cylinder(d=5, h=6);

    translate([-2.8/2, 9.8/2,0]) cube([2.8,6,0.3]);
  }
}

module batterieEinzeln()
{
  translate([0,-1,0]) feder();
  translate([0,-1,52]) rotate([0,180,0]) kontakt();
  translate([0,0,6]) aaa();
}

module batteriefach()
{
  difference()
  {
    translate([-12.5,-8,-2]) cube([25, 12, 56]);
    
    translate([-11,-6,2]) cube([22, 20, 48]);
    
    translate([-10.5,-7,-.25]) cube([10,12,0.8]);
    translate([  0.5,-7,-.25]) cube([10,12,0.8]);
    
    translate([-9,-6,0]) cube([7,12,3]);
    translate([ 2,-6,0]) cube([7,12,3]);
    
    translate([-10.5,-7,52-.25]) cube([10,12,0.8]);
    translate([  0.5,-7,52-.25]) cube([10,12,0.8]);
    translate([-9,-6,49]) cube([7,12,3]);
    translate([ 2,-6,49]) cube([7,12,3]);
  }

  *difference()
  {
    translate([-11,-6,0]) cube([22,10,2]);
    
    translate([-10.5,-7,0]) cube([10,12,0.8]);
    translate([  0.5,-7,0]) cube([10,12,0.8]);
  }
}

*rotate([90,0,0])
batteriefach();

*translate([5.5,0,0]) batterieEinzeln();
*translate([-5.5,0,52]) rotate([0,180,0]) batterieEinzeln();

