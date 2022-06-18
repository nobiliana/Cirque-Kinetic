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


//Take Final delta values pre-liftoff, apply kinetic. May need to pull LAST deltas, as liftoff detection may occur at same time of x/y wipe. 
float xVal = 30; //will be int in the final op, currently a test value. Will be equal to x delta.
float yVal = 40; //will be int in the final op, currently a test value. Will be equal to y delta. 

//indicated in decimal value
#define friction 0.35

bool LIFTOFF = TRUE; //False being contact, True being finger off. 
bool kineticInit = TRUE; //variable to initialize the kinetic values before start. "inverted" to make logic more visually correct

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