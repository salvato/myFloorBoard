// Measurements in mm
$fn = 80;

font1 = "FreeSans:style=Bold";

wallThickness = 2.0;

edgeRad = 10;
base    = 150;
minH    = 35;

width   = 150;

center  = true;
left    = false;
right   = false;

upper   = false;
lower   = false;

pushButton = 13;

slope = atan2(edgeRad, base);


module 
shap(){
    polygon(points=[[0, 0],
                     [base, 0],
                     [base, minH+edgeRad],
                     [0, minH]]);
    /*
    hull() {
        translate([edgeRad, edgeRad, 0])
            circle(r = edgeRad);
        translate([0, base-edgeRad, 0])
            circle(r = edgeRad);
    }
    polygon(points=[[0, edgeRad],
                     [-edgeRad, base-edgeRad],
                     [0, base],
                     [minH, base],
                     [minH, 0],
                     [edgeRad, 0]]);
    
*/
}



module
pedal_center(textLeft, textRight, withLCD, withMIDI) {
    union() {
        difference() {
            union() {//1   
                // Carcassa
                translate([0, 0, 0])
                    rotate([90, 0, 0])
                        linear_extrude(height = width)
                            difference() {
                                offset(r = wallThickness)
                                    shap();
                                shap();
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

