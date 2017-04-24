/**
 * Case for Capacitive Soil Sensors
 * 
 * License: Creative Commons Attribution-ShareAlike 4.0 International
 * Copyright 2017 Alan Lord
**/
$fn = 50;

l = 80;
w = 35;
h = 43;

prbw = 17;
prbd = 2;

wall = 2;

nutSize = 6;
nutHeight = 3;
nutThread = 3.2;
boltLength = 8;
fix=8;

pcbw = 29.5;
pcbh = 52.5;
pcbpad = 6.5;

pcbwctrs = 20.320;
pcbdctrs = 51.435;

sloth=((w-pcbw)/2)+1.5;

slotDepthFromTop = 15;

// Display the box
box();

// Display the lid
%lid();

module box() {
    %difference() { // The % makes the external wall transparent ot invisibble
        roundedCube(h+wall*2,w+wall*2,l+wall,nutThread);
        translate([wall,wall,wall]) {
            roundedCube(h,w,l,nutThread);
        }
        translate([slotDepthFromTop,((w+wall*2)-prbw)/2,0]) {
            cube([prbd,prbw,wall*2]);
        }
    }
    translate([slotDepthFromTop-wall,wall,wall]) {
        pcbSlot(pcbh);
    }
    translate([slotDepthFromTop-wall,w+wall-sloth,wall]) {
        pcbSlot(pcbh);
    }
    translate([slotDepthFromTop+wall*3,wall,wall]) {
        cube([1,w,l-wall*2]);
    }
    fixing();
    translate([0,(w+wall*2)-fix,0]) {
        fixing();
    }
    translate([(h+wall*2)-fix,(w+wall*2)-fix,0]) {
        fixing();
    }
    translate([(h+wall*2)-fix,0,0]) {
        fixing();
    }
    
    x=(h-22+wall);
    y=((w+wall*2)-21)/2;
    translate([x,y,wall]) {
        battery();
    }
}


module lid() {
    xtrns = nutHeight+wall+8.5;
    difference(){
        translate([0,0,l+wall]) {
            roundedCube(h+wall*2,w+wall*2,wall,nutThread);
        }
        translate([fix/2,fix/2,l+wall]) {
            cylinder(d=nutThread,h=wall);
        }
        translate([(h+wall*2)-fix/2,fix/2,l+wall]) {
            cylinder(d=nutThread,h=wall);
        }
        translate([(h+wall*2)-fix/2,(w+wall*2)-fix/2,l+wall]) {
            cylinder(d=nutThread,h=wall);
        }
        translate([fix/2,(w+wall*2)-fix/2,l+wall]) {
            cylinder(d=nutThread,h=wall);
        }
    }
    sloth=l-(pcbh+pcbpad)-1;
    translate([slotDepthFromTop-wall,((w+wall*2)-w)/2,l-sloth+wall]) {
        difference() {
            cube([wall*3,w,sloth]);
            translate([0,w/2,0]) {
                rotate([90,0,90]) cylinder(h=wall*3,d=5);
            }
        }
    }
    translate([h-10,(w+wall*2)/2,(l+wall)-3]) {
        cylinder(h=3,d=10);
    }

}

module bolt(width,height,thread,length,hull) {
    cylinder(d=width/(cos(180/6)), h=height, $fn=6);
    if(hull) {
        hull(){
            translate([0,0,height]) {
                cylinder(d=thread,h=length);
            }
            translate([thread,0,height]) {
                cylinder(d=thread,h=length);
            }
        }
    } else {
        translate([0,0,height]) {
            cylinder(d=thread,h=length);
        }
    }
}

module fixing() {
    difference() {
        translate([0,0,wall]){
            roundedCube(fix,fix,l,nutThread);
        }
        translate([fix/2,fix/2,l-(boltLength-wall)]) {
            cylinder(d=nutThread,h=boltLength);
        }
    }
}

module battery() {
    l=76;
    w=21;
    h=22;
    cube([h,w,l]);
}

module pcbSlot(l) {
    cube([wall,sloth,l+pcbpad]);
    translate([wall*2,0,0]) {
        cube([wall,sloth,l+pcbpad]);
    }
    translate([wall,0,0]) {
        cube([prbd,sloth,pcbpad]);
    }
}

module roundedCube(xdim,ydim,zdim,rdim) {
    hull() {
        translate([rdim,rdim,0]) cylinder(r=rdim,h=zdim);
        translate([xdim-rdim,rdim,0]) cylinder(r=rdim,h=zdim);
        translate([rdim,ydim-rdim,0]) cylinder(r=rdim,h=zdim);
        translate([xdim-rdim,ydim-rdim,0]) cylinder(r=rdim,h=zdim);
    }
}