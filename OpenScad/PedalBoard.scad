$fn = 80;


edgeRad = 10;
base     = 150;
minH    = 35;

width   = 150;

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


//difference() {
union() {
    translate([0, 0, minH])
        rotate([90, 90, 0])
            linear_extrude(height = width)
                difference() {
                    offset(r = 5)
                        shap();
                    shap();
                }

    union(){
        translate([25, -25, 25])
            rotate([0, -16, 0])
                cylinder(d = 7.7, h = 10);
        translate([45,-40,6])
            rotate([0,90,0])
            cylinder(r = 6.3/2, h = 10);
        translate([45,-11,11])
            rotate([0,90,0])
                cylinder(r = 10.25/2, h = 10);
    }
}
