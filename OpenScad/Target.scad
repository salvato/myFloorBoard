font1 = "FreeSans:style=Bold";

union() {
    color("Blue")
    linear_extrude(height = 1.0) {
        text("Up",
        font = font1, 
        size = 10, 
        halign = "center");
    }
    cube([20, 20, 0.2], center=true);

    translate([30, 0, 0]) {
        color("Blue")
        linear_extrude(height = 1.0) {
            text("Dn",
            font = font1, 
            size = 10, 
            halign = "center");
        }
        cube([20, 20, 0.2], center=true);
    }

    translate([-30, 30, 0]) {
        color("Blue")
        linear_extrude(height = 1.0) {
            text("A",
            font = font1, 
            size = 10, 
            halign = "center");
        }
        cube([20, 20, 0.2], center=true);
    }

    translate([0, 30, 0]) {
        color("Blue")
        linear_extrude(height = 1.0) {
            text("B",
            font = font1, 
            size = 10, 
            halign = "center");
        }
        cube([20, 20, 0.2], center=true);
    }

    translate([30, 30, 0]) {
        color("Blue")
        linear_extrude(height = 1.0) {
            text("C",
            font = font1, 
            size = 10, 
            halign = "center");
        }
        cube([20, 20, 0.2], center=true);
    }

    translate([60, 30, 0]) {
        color("Blue")
        linear_extrude(height = 1.0) {
            text("D",
            font = font1, 
            size = 10, 
            halign = "center");
        }
        cube([20, 20, 0.2], center=true);
    }
}