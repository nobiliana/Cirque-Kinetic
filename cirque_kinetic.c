/*
initial testing with online C compilers. integration with zmk/qmk converters required, and change variables to be time vs. loop based.

add functionality for friction modifiers and keys, high and low friction to allow long glide or fast stop. 
    Change primary functionality of friction to be less x/y based and mainly magnitude, so Radial can be passed in. 
*/

#include <stdio.h>
#include <math.h>
#include "cirque_kinetic.h"

//indicated in decimal value
#define friction 0.35

bool LIFTOFF = TRUE; //False being contact, True being finger off. 
bool kineticInit = TRUE; //variable to initialize the kinetic values before start. "inverted" to make logic more visually correct

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



int kineticCirque()
{   
    if (LIFTOFF){ 
        if (kineticInit){ //initialize the vector values. ensures it is run once ONLY per liftoff event. 
            kineticVector(xVal, yVal); //will take deltaX and deltaY from drivers, and calculate into the xPoint and yPoints.
            kineticInit = FALSE;
        }

        if (mVector.magValue > 0){
            //printf("loop entered. logic to follow. \n");            
                mVector.magValue = kineticDrag(mVector.angValue, mVector.magValue);
                /*
                BREAKOUT POSITION FOR TAKING XPOINT AND YPOINT TO FIRMWARE POINTER CODE. GUARDS IN PLACE FOR NEGATIVE MAGNITUDES.
                */
        } 
    } else if (!LIFTOFF){
        if (!kineticInit){
            kineticVector(0, 0); //reinitialize to zero. ensure we have no residual data. 
            kineticInit = TRUE; //if finger is touching, reset init lock. 
        }
    }
    
    return 0;
}