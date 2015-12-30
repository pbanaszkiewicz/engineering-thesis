//  ----------------------------------------------------------------------- LICENSE
//  This "3D Printed Case for Arduino Uno, Leonardo" by Zygmunt Wojcik is licensed
//  under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
//  To view a copy of this license, visit
//  http://creativecommons.org/licenses/by-sa/3.0/
//  or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.


include <containerA_params.scad>

//------------------------------------------------------------------------- MODULES
module pcbLeg() {		
	translate([0, 0, 0])
	difference() { 											
		cylinder(h = floorHeight + pcbPositionZ, r1=5.5/2, r2 = 4.5/2);
	}
}


//------------------------------------------------------------------------- MAIN BLOCK
difference()
{
																		// ADD
	union()
	{
		// Add Base
		linear_extrude(height = height/2, convexity = 10)
		minkowski()
		{									
			square([width, wide], center = true);
			circle(roundR);
		}
	}
																		// SUBSTRACT
	union()
	{
		// Lift floor height
		translate([0, 0, floorHeight])
		{
			// Cut Base hole
			linear_extrude(height = height/2, convexity = 10)
			minkowski()
			{
				square([width, wide], center = true);
				circle(roundR - pillarSize);
			}
			// Cut upper block lock
			difference() {
				translate([0, 0, height/2 - floorHeight - blockLockSize]) {
					cylinder(h = blockLockSize+gap, r=width);
				}
				translate([0, 0, height/2 - floorHeight - blockLockSize - gap*2]) {
					linear_extrude(height = blockLockSize+gap*4, convexity = 10)
					minkowski() {
						square([width, wide], center=true);
						circle(roundR - layerWidth*4);
					}
				}
			}
			// Cut x panels 
			for (i = [0 : 180 : 180])				
			rotate([0, 0, i])
			translate([width/2 + roundR - pillarSize/2 - layerWidth*7, 0, 0])
			{
				// Cut X panel hole
				translate([0, 0, height/2])
				cube([pillarSize, sidePanelXWidth, height], center=true);
                
				// Cut X, Y srew holes
				for (i = [wide/2, -wide/2])
				{
					translate([-(roundR - pillarSize/2 - layerWidth*7), i, - floorHeight])
					if (i>0) 
					{
						rotate([0, 0, 45])
						translate([screwHoleRoundR, 0, 0])
						{
							cylinder(h = height*2, r=screwExt/2, center=true);
							cylinder(h = 5,
                                    r1 = (screwHead + (screwHead - screwExt))/2,
                                    r2 = screwExt/2, center=true);
						}
					}
					else
					{
						rotate([0, 0, -45])
						translate([screwHoleRoundR, 0, 0])
						{
							cylinder(h = height*2, r=screwExt/2, center=true);
							cylinder(h = 5,
                                    r1 = (screwHead + (screwHead - screwExt))/2,
                                    r2 = screwExt/2, center=true);
						}
					}
				}
			}
			// Cut Y panels 
			for (i = [90 : 180 : 270])
			rotate([0, 0, i])
			translate([wide/2 + roundR - pillarSize/2 - layerWidth*7, 0, 0])
			{
				// Cut Y panel hole
				translate([0, 0, height/2])
				cube([pillarSize, sidePanelYWidth, height], center=true);
			}
		}
	}
}
//------------------------------------------------------------------------- ADD PCB LEGS
// Translate to pcbPositionX	
translate([-pcbPositionX, -pcbWide/2, 0])

difference()
{
																		// ADD
	union()
	{
		// Add pcb legs
		for(i=[ [10.05, 2.54, 0],
     	    	[10.05, 50.8, 0],
          		[57.04, 35.56, 0],
     	    	[57.04, 7.62, 0] ])
		{
	    		translate(i)
	    		pcbLeg();
		}
	}
																		// SUBSTRACT
	union()
	{
		// Add pcb holders
		for(i=[ [10.05, 2.54, 0],
     	    	[10.05, 50.8, 0],
          		[57.04, 35.56, 0],
     	    	[57.04, 7.62, 0] ])
		{
			translate(i)
			cylinder(h=floorHeight+pcbPositionZ+1.5, r=1.2);
		}
	}
}