// Measurements in mm
$fn = 80;

font1 = "FreeSans:style=Bold";

wallThickness = 2.0;

base = 150;
minH = 35;
maxH = 45;
width = 150;

center = false;
left = false;
right = false;

upper = false;
lower = false;

pushButton = 13;

slope = atan2(maxH-minH, base);


module 
profile() {
    polygon(points=[[0, 0],
                     [base, 0],
                     [base, maxH],
                     [0, minH]]);
}


module
container() {
    translate([0, width, 0])
    rotate([90, 0, 0])
        linear_extrude(height = width)
            difference() {
                offset(r = wallThickness)
                    profile();
                profile();
            }    
}


module
LCD() {
    difference() {
        union() {
            color([1, 0, 0])
                cube([60, 100, 1.5]);
            color([0, 0, 1])
            translate([9, 0, 1.5])
                cube([42, 100, 8]);
        }
        translate([4, 4, -0.5])
        cylinder(d=3, h=2.5);
        translate([56, 4, -0.5])
        cylinder(d=3, h=2.5);
        translate([4, 96, -0.5])
        cylinder(d=3, h=2.5);
        translate([56, 95, -0.5])
        cylinder(d=3, h=2.5);
    }
}


union() {
    //container();
    translate([(base-70), 0.5*(base-100), minH-3+sin(slope)*(base-70)]) {
        translate([4, 4, 0])
            cylinder(d=3, h=5);
        translate([4, 96, 0])
            cylinder(d=3, h=5);
    }
    translate([(base-70), 0.5*(base-100), minH-3+sin(slope)*(base-18)]) {
        translate([56, 4, 0])
            cylinder(d=3, h=5);
        translate([56, 95, 0])
            cylinder(d=3, h=5);
    }
    translate([(base-70), 0.5*(base-100), minH-5.5+sin(slope)*(base-70)])
        rotate([0, -slope, 0])
            LCD();
}


module
pedal_center(textLeft, textRight, withLCD, withMIDI) {
    union() {
        difference() {
            union() {//1   
                // Carcassa
                rotate([90, 0, 0])
                    linear_extrude(height = width)
                        difference() {
                            offset(r = wallThickness)
                                profile();
                            profile();
                        }
                if(withLCD) {
                    // Supporto viti LCD
                    translate([74, -28.5, 40-4])
                        rotate([0, -slope, 0])
                            difference() {
                                union() {
                                    translate([0, 0, 0])
                                        cylinder(d = 6, h = 5);
                                    translate([0, -93, 0])
                                        cylinder(d = 6, h = 5);
                                    translate([54, 0, 0])
                                        cylinder(d = 6, h = 5);
                                    translate([54, -93, 0])
                                        cylinder(d = 6, h = 5);
                                }
                                // Fori viti LCD                
                                union() {
                                    translate([0, 0, -0.5])
                                        cylinder(d = 3, h = 6.5);
                                    translate([0, -93, -0.5])
                                        cylinder(d = 3, h = 6.5);
                                    translate([54, 0, -0.5])
                                        cylinder(d = 3, h = 6.5);
                                    translate([54, -93, -0.5])
                                        cylinder(d = 3, h = 6.5);
                                }
                            }
                }
            }// end union() 1
            
            union() {//2
                if(withLCD) {
                    // Scasso LCD
                    color([0, 0, 1])
                    translate([80, -125, 38])
                        rotate([0, -slope, 0])
                            #cube([42, 100, 8]);
                    translate([71, -125, 35])
                        rotate([0, -slope, 0])
                            #cube([60, 100, 1.5]);
                }
                if(withMIDI) {
                    // Scasso MIDI In
                    translate([base+.2, -45, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                    // Scasso MIDI Out
                    translate([base+.2, -105, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                        
                }
                // Scasso pulsante 1
                translate([35, -35, 34]) {
                    rotate([0, -slope, 0]) {
                        #cylinder(d = pushButton, h = 8);
                        translate([15, 0, 0]) {
                            rotate([0, -0, -90])
                                #linear_extrude(height = 5.0) {
                                    text(textLeft,
                                    font = font1, 
                                    size = 10, 
                                    halign = "center");
                                }
                        }
                    }
                }
                // Scasso pulsante 2
                translate([35, -115, 34]) {
                    rotate([0, -slope, 0]) {
                        #cylinder(d = pushButton, h = 8);
                        translate([15, 0, 0]) {
                            rotate([0, -0, -90])
                                #linear_extrude(height = 5.0) {
                                    text(textRight,
                                    font = font1, 
                                    size = 10, 
                                    halign = "center");
                                }
                        }
                    }
                }
            }// end union() 2
        }// end difference() 1
        /* Rinforzi
        rotate([0, -slope, 0])
            translate([57, -width+20, 30]) 
                cube([1.0, width-40.0, 4.0]);
        rotate([0, -slope, 0])
            translate([49, -35.5, 30]) 
                cube([15.0, 1.0, 4.0]);
        rotate([0, -slope, 0])
            translate([49, -115.5, 30]) 
                cube([15.0, 1.0, 4.0]);
        */
    }
}


if(center) {
    if(upper) {
        difference() {
            pedal_center("C", "D", true, true);
            translate([-25, -175, -9.5])
                #cube([200, 200, 15]);
        }
    }
    else if(lower) {
        intersection() {
            pedal_center("C", "D", true, true);
            translate([-25, -175, -9.5])
                #cube([200, 200, 15]);
        }
    }
    else {
        pedal_center("C", "D", true, true);
    }
}


if(left) {
    if(upper) {
        difference() {
            translate([0, width, 0])
                pedal_center("A", "B", false, false);
            translate([-25, width-175, -9.5])
                #cube([200, 200, 15]);
        }
    } 
    else if(lower) {
        intersection() {
            translate([0, width, 0])
                pedal_center("A", "B", false, false);
            translate([-25, width-175, -9.5])
                #cube([200, 200, 15]);
        }
    }
    else {
        translate([0, width, 0])
            pedal_center("A", "B", false, false);
    }
}


if(right) {
    if(upper) {
        difference() {
            translate([0, -width, 0])
                pedal_center("Up", "Dn", false, false);
            translate([-25, -width-175, -9.5])
                #cube([200, 200, 15]);
        }
    } 
    else if(lower) {
        intersection() {
            translate([0, -width, 0])
                pedal_center("Up", "Dn", false, false);
            translate([-25, -width-175, -9.5])
                #cube([200, 200, 15]);
        }
    }
    else {
        translate([0, -width, 0]) {
            pedal_center("Up", "Dn", false, false);
        }
    }
}

