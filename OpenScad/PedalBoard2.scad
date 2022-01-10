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
lcdPos = [depth-70, 0.5*(width-100)+15.0, (minH-6.5+sin(slope)*(depth-70))];
lHeigth = 20;

// Cosa disegnare ?
center = true;
left = false;
right = false;

upper = false;
lower = true;

//
// Moduli
//
module 
profile() {
    polygon(points=[[0, 0],
                     [depth, 0],
                     [depth, maxH],
                     [0, minH]]);
}
//
//
module
rinforzi() {
    union() {
        // Rinforzi Top
        color("Blue") {
            rotate([0, -slope, 0])
                translate([0.5*wallThickness, 0, minH-0.95*7.0]) 
                    cube([depth+wallThickness, 3.0, 7.0]);
            rotate([0, -slope, 0])
                translate([0.5*wallThickness, width-3.0, minH-0.95*7.0]) 
                    cube([depth+wallThickness, 3.0, 7.0]);
            rotate([0, -slope, 0])
                translate([lcdPos.x-3.0, 0.5*wallThickness, minH-0.95*7.0]) 
                    cube([3.0, width-wallThickness, 7.0]);
        }
        // Rinforzi Bottom
        color("Red") {
            difference() {
                union() {
                    translate([-0.5*wallThickness, width-7.0, -0.5*wallThickness]) 
                        cube([depth+wallThickness, 7.0, 5.5]);
                    translate([20, width-7.0, 5.0-0.5*wallThickness])
                        rotate([-90, 0, 0])
                            cylinder(d=9, h=7);
                    translate([depth-20, width-7.0, 5.0-0.5*wallThickness])
                        rotate([-90, 0, 0])
                            cylinder(d=9, h=7);
                }
                translate([20, width-7.5, 5.0-0.5*wallThickness])
                    rotate([-90, 0, 0])
                        cylinder(d=3, h=8);
                translate([depth-20, width-7.5, 5.0-0.5*wallThickness])
                    rotate([-90, 0, 0])
                        cylinder(d=3, h=8);
            }
            difference() {
                union() {
                    translate([-0.5*wallThickness, 0, -0.5*wallThickness]) 
                        cube([depth+wallThickness, 7.0, 5.5]);
                    translate([20, 0, 5.0-0.5*wallThickness])
                        rotate([-90, 0, 0])
                            cylinder(d=9, h=7);
                    translate([depth-20, 0, 5.0-0.5*wallThickness])
                        rotate([-90, 0, 0])
                            cylinder(d=9, h=7);
                }
                translate([20, -0.5, 5.0-0.5*wallThickness])
                    rotate([-90, 0, 0])
                        cylinder(d=3, h=8);
                translate([depth-20, -0.5, 5.0-0.5*wallThickness])
                    rotate([-90, 0, 0])
                        cylinder(d=3, h=8);
            }
        }
        // Alloggiamenti viti fissaggio pannelli
        color("Yellow") {
            translate([4.0, 5.5, 0])
                cylinder(d=10, h=minH);
            translate([depth-4.0, 5.5, 0])
                cylinder(d=10, h=maxH);
            translate([4.0, width-5.5, 0])
                cylinder(d=10, h=minH);
            translate([depth-4.0, width-5.5, 0])
                cylinder(d=10, h=maxH);
        }
    }
}
//
//
module
foriFissaggio() {
    union() {
        // Alloggiamenti viti fissaggio pannelli
        color("Yellow") {
            translate([4, 5.5, -3*wallThickness])
                cylinder(d=4, h=minH);
            translate([depth-4, 5.5,  -3*wallThickness])
                cylinder(d=4, h=maxH);
            translate([4, width-5.5, -3*wallThickness])
                cylinder(d=4, h=minH);
            translate([depth-4, width-5.5, -3*wallThickness])
                cylinder(d=4, h=maxH);
        }
    }
}
//
//
module
container() {
    difference() {
        union() {
            translate([0, width, 0])
            rotate([90, 0, 0])
                linear_extrude(height = width)
                    difference() {
                        offset(r = wallThickness)
                            profile();
                        profile();
                    }
            rinforzi();
        }
        foriFissaggio();
        // Authorship
        translate([0.5*depth+20, 0.5*width, -1.0]) {
            rotate([0, -0, -90])
                #linear_extrude(height = 1.5) 
                    //text("© Gabriele",
                    text("Gabriele",
                    //font = "URW Chancery L:style=bold",
                    font = "Purisa:style=bold", 
                    size = 10, 
                    halign = "center");
        translate([-20, 0, 0])
            rotate([0, -0, -90])
                #linear_extrude(height = 1.5) 
                    text("Salvato",
                    //font = "URW Chancery L:style=bold",
                    font = "Purisa:style=bold", 
                    size = 10, 
                    halign = "center");
        }
    }    
}
//
//
module
LCD() {
    difference() {
        union() {
            color([1, 0, 0])
                cube([60, 98, 1.6]);//PCB
            color([0, 0, 1])
                translate([9, 0, 1.5])
                    cube([42, 98, 8]);//LCD
        }
        diam = 3.0;
        offset = 2.5;
        translate([offset, offset, -0.5])
            cylinder(d=diam, h=2.5);
        translate([60-offset, 2, -0.5])
            cylinder(d=diam, h=2.5);
        translate([offset, 98-offset, -0.5])
            cylinder(d=diam, h=2.5);
        translate([60-offset, 98-offset, -0.5])
            cylinder(d=diam, h=2.5);
    }
}
//
//
module
LCDsupport(heigth) {
    difference() {
        offset = 2.5;
        union() {// Distanziatori
            translate([offset, offset, -0.5])
                cylinder(d=6, h=heigth);
            translate([60-offset, offset, -0.5])
                cylinder(d=6, h=heigth);
            translate([offset, 98-offset, -0.5])
                cylinder(d=6, h=heigth);
            translate([60-offset, 98-offset, -0.5])
                cylinder(d=6, h=heigth);
        }
        union() {// Fori per viti
            diam = 2.0;
            translate([offset, offset, -1.5])
                cylinder(d=diam, h=heigth+3);
            translate([60-offset, offset, -1.5])
                cylinder(d=diam, h=heigth+3);
            translate([offset, 98-offset, -1.5])
                cylinder(d=diam, h=heigth+3);
            translate([60-offset, 98-offset, -1.5])
                cylinder(d=diam, h=heigth+3);
        }
    }
}
//
//
module
pedal_center(textLeft, textRight, withLCD, withMIDI) {
        difference() {
            union() {//1
                // Carcassa
                container();
                if(withLCD) {
                    // Supporto viti LCD
                    translate(lcdPos+[0, 0, 3.0])
                        rotate([0, -slope, 0])
                            LCDsupport(6);
                }
            }// end union() 1
            //Scassi
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
                        translate([15, 0, 0])
                            rotate([0, -0, -90])
                                #linear_extrude(height = 5.0) 
                                    text(textLeft,
                                    font = font1, 
                                    size = 10, 
                                    halign = "center");
                     
                    }
                }
                // Scasso pulsante 2
                translate([width/4, width/4, 34]) {
                    rotate([0, -slope, 0]) {
                        #cylinder(d = pushButton, h = 8);
                        translate([15, 0, 0])
                            rotate([0, -0, -90])
                                #linear_extrude(height = 5.0)
                                    text(textRight,
                                    font = font1, 
                                    size = 10, 
                                    halign = "center");
                    }
                }
                // Scassi connettori MIDI
                if(withMIDI) {
                    // Scasso MIDI Out
                    yPos1 = 28;
                    translate([depth+.2, yPos1, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                    //translate([depth-2*wallThickness, yPos1-11.5, 22])
                        //rotate([0, 90, 0])
                            //#cylinder(r=2.5, h=4*wallThickness);
                    //translate([depth-2*wallThickness, yPos1+11.5, 22])
                        //rotate([0, 90, 0])
                            //#cylinder(r=2.5, h=4*wallThickness);
                    // Scasso MIDI In
                    yPos2 = 62;
                    translate([depth+.2, yPos2, 22])
                        rotate([0, 90, 0])
                            #cylinder(r=8.5, h=wallThickness-.4);
                    //translate([depth-2*wallThickness, yPos2-11.5, 22])
                        //rotate([0, 90, 0])
                            //#cylinder(r=2.5, h=4*wallThickness);
                    //translate([depth-2*wallThickness, yPos2+11.5, 22])
                        //rotate([0, 90, 0])
                            //#cylinder(r=2.5, h=4*wallThickness);
                }
            }// end union() 2
        }// end difference() 1    
}
//
//
if(center) {
    if(upper) {
        difference() {
            pedal_center("C", "D", true, true);
            translate([-0.25*depth, -0.25*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    }
    else if(lower) {
        intersection() {
            pedal_center("C", "D", true, true);
            translate([-0.25*depth, -0.25*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    }
    else {
        pedal_center("C", "D", true, true);
    }
}
//
//
if(left) {
    if(upper) {
        difference() {
            translate([0, width, 0])
                pedal_center("A", "B", false, false);
            translate([-0.25*depth, 0.75*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    } 
    else if(lower) {
        intersection() {
            translate([0, width, 0])
                pedal_center("A", "B", false, false);
            translate([-0.25*depth, 0.75*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    }
    else {
        translate([0, width, 0])
            pedal_center("A", "B", false, false);
    }
}
//
//
if(right) {
    if(upper) {
        difference() {
            translate([0, -width, 0])
                pedal_center("Up", "Dn", false, false);
            translate([-0.25*depth, -1.25*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    } 
    else if(lower) {
        intersection() {
            translate([0, -width, 0])
                pedal_center("Up", "Dn", false, false);
            translate([-0.25*depth, -1.25*width, -9.5])
                cube([1.5*depth, 1.5*width, lHeigth]);
        }
    }
    else {
        translate([0, -width, 0]) {
            pedal_center("Up", "Dn", false, false);
        }
    }
}

