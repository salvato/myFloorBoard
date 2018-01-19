$fn = 80;


edgeRad = 10;
base     = 150;
minH    = 35;

width   = 150;

upper = true;

module shap(){
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
}

difference() {
    union() {   
        // Carcassa
        translate([0, 0, minH])
            rotate([90, 90, 0])
                linear_extrude(height = width)
                    difference() {
                        offset(r = 5)
                            shap();
                        shap();
                    }
        // Supporto viti LCD
        translate([74, -28.5, 38])
            rotate([0, -4.5, 0])
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
                    #union() {
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
    #union() {
        // Scasso LCD
        color([0, 0, 1])
        translate([80, -125, 38])
            rotate([0, -4.5, 0])
                cube([42, 100, 8]);
        translate([71, -125, 36])
            rotate([0, -4.5, 0])
                cube([60, 100, 1.5]);
        // Scasso MIDI In
        color("Silver") {
        translate([145, -60, 22])
            rotate([0, 90, 0])
            cylinder(r = 8.5, h = 15);
        // Scasso MIDI Out
        translate([145, -90, 22])
            rotate([0, 90, 0])
            cylinder(r = 8.5, h = 15);
        // Scasso pulsante 1
        translate([35, -35, 25])
            rotate([0, -4.5, 0])
                cylinder(d = 13, h = 25);
        // Scasso pulsante 2
        translate([35, -115, 25])
            rotate([0, -4.5, 0])
                cylinder(d = 13, h = 25);
        }
    }
    if(upper) {
        #translate([-25, -175, -9.5])
            cube([200, 200, 15]);
    }
}
