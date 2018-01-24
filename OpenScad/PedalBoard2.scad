// Measurements in mm
$fn = 80;

font1 = "FreeSans:style=Bold";

wallThickness = 2.0;
depth = 150;
width = 150;
minH = 35;
maxH = 45;
pushButton = 13;// Diametro pulsanti
slope = atan2(maxH-minH, depth);// pendenza superficie superiore
lcdPos = [depth-70, 0.5*(width-100), (minH-5.5+sin(slope)*(depth-70))];

// Cosa disegnare ?
center = true;
left = false;
right = false;

upper = true;
lower = false;


// Moduli
module 
profile() {
    polygon(points=[[0, 0],
                     [depth, 0],
                     [depth, maxH],
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
        translate([56, 96, -0.5])
        cylinder(d=3, h=2.5);
    }
}


module
LCDsupport(heigth) {
    difference() {
        union() {
            translate([4, 4, -0.5])
                cylinder(d=6, h=heigth);
            translate([56, 4, -0.5])
                cylinder(d=6, h=heigth);
            translate([4, 96, -0.5])
                cylinder(d=6, h=heigth);
            translate([56, 96, -0.5])
                cylinder(d=6, h=heigth);
        }
        union() {
            translate([4, 4, -0.5])
                cylinder(d=2, h=heigth+1);
            translate([56, 4, -0.5])
                cylinder(d=2, h=heigth+1);
            translate([4, 96, -0.5])
                cylinder(d=2, h=heigth+1);
            translate([56, 96, -0.5])
                cylinder(d=2, h=heigth+1);
        }
    }
}




module
pedal_center(textLeft, textRight, withLCD, withMIDI) {
    union() {
        difference() {
            union() {  
                // Carcassa
                container();
                if(withLCD) {
                    // Supporto viti LCD
                    translate(lcdPos+[0, 0, 1.5])
                        rotate([0, -slope, 0])
                            LCDsupport(5);
                }
            }
            
            union() {//2
                // Scasso LCD
                if(withLCD) {
                    translate(lcdPos)
                        rotate([0, -slope, 0])
                            #LCD();
                }
                // Scasso pulsante 1
                translate([width/4, 3*width/4, 34]) {
                    rotate([0, -slope, 0]) {
                        #cylinder(d = pushButton, h = 8);
                        // Etichetta pulsante 1
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
                translate([width/4, width/4, 34]) {
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
                // Scassi connettori MIDI
                if(withMIDI) {
                    // Scasso MIDI In
                    translate([depth+.2, 3*width/4, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                    // Scasso MIDI Out
                    translate([depth+.2, width/4, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                        
                }
            }// end union() 2
        }// end difference() 1
        
        // Rinforzi
        color("Blue")
            rotate([0, -slope, 0])
                translate([0.5*wallThickness, 0, minH-0.95*7.0]) 
                    cube([depth+wallThickness, 3.0, 7.0]);
        color("Blue")
            rotate([0, -slope, 0])
                translate([0.5*wallThickness, width-3.0, minH-0.95*7.0]) 
                    cube([depth+wallThickness, 3.0, 7.0]);

        color("Red")
            rotate([0, -slope, 0])
                translate([lcdPos.x-10.0, 0.5*wallThickness, minH-0.95*7.0]) 
                    cube([3.0, width-wallThickness, 7.0]);


    }
}


if(center) {
    if(upper) {
        difference() {
            pedal_center("C", "D", true, true);
            translate([-0.25*depth, -0.25*width, -9.5])
                cube([1.5*depth, 1.5*width, 15]);
        }
    }
    else if(lower) {
        intersection() {
            pedal_center("C", "D", true, true);
            translate([-0.25*depth, -0.25*width, -9.5])
                #cube([1.5*depth, 1.5*width, 15]);
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

