/*
initial testing with online C compilers. integration with zmk/qmk converters required, and change variables to be time vs. loop based.

add functionality for friction modifiers and keys, high and low friction to allow long glide or fast stop. 
    Change primary functionality of friction to be less x/y based and mainly magnitude, so Radial can be passed in. 
*/

#include <stdio.h>
#include <math.h>
#include "cirque_kinetic.h"

//friction, needs grav constant. a = gu
const float grav = 9.82;
const float PI = 3.14159;
const float rad2deg = 180/PI;

float xVal = 30; //will be int in the final op, error checking with float.
float yVal = 40; //will be int in the final op, error checking with float.

//indicated in decimal value
#define friction 0.35

int LIFTOFF = 0; //1 being contact, 0 being finger off. 

typedef struct {
    int xPoint;
    int yPoint;
    float magValue;
    float angValue;
} mouseThings;

mouseThings mVector = {0};

//friction function
float kineticDrag (float vecAngle, float vecMagn){
    if (vecMagn - grav*friction <=0){
        mVector.xPoint = 0;
        mVector.yPoint = 0;
    } else {
        mVector.xPoint = cos(vecAngle)*vecMagn; //apply floor to the calculation for final int. 
        mVector.yPoint = sin(vecAngle)*vecMagn; //apply floor to the calculation for final int. 
    }
    return vecMagn - grav*friction;
}

//inputs to this should be x and y vectors!
void kineticVector (int xMouse, int yMouse){
    //setting vector magnitude and angle
    mVector.magValue = sqrt((xMouse*xMouse)+(yMouse*yMouse));
    mVector.angValue = atan2(yMouse, xMouse);
}



int main()
{   //change functions away from loops, as not necessary in QMK. 
    while(liftTest>0){ //test loop declaration. not necessary for final code. will need replacement with a liftoff detection bool      
        kineticVector(xVal, yVal); //will take deltaX and deltaY from drivers, and calculate into the xPoint and yPoints. 
        while(mVector.magValue > 0 && !LIFTOFF){
            //printf("loop entered. logic to follow. \n");            
                mVector.magValue = kineticDrag(mVector.angValue, mVector.magValue);            
        }
    }
    
    return 0;
}