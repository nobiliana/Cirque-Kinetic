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

//test loop only
int loopCount = 0;  
int loopCycle = 3; //tester value, to simulate time check loops. implied time check loop
int LIFTOFF = 0; //1 being contact, 0 being finger off. 
int liftTest = 3; //phase 2 test, 2 lift test. lift 1 will reset same xVal and yVal, lift 2 will change angle. 
//test loop end

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
{
    printf("Hello World. \n");
//    printf("test: %f\n", kineticVector(5, 6));

    while(liftTest>0){ //test loop declaration. not necessary for final code. 
        
        kineticVector(xVal, yVal);
    
        printf("friction coefficient: %f\n", friction);
        printf("vector magnitude: %f\n", mVector.magValue);
        printf("vector angle (radians): %f\n", mVector.angValue);
        printf("vector angle (degree): %f\n", mVector.angValue*rad2deg);
        
        printf("Begin kinetic loop \n");
        printf("\n");
        

        while(mVector.magValue > 0 && !LIFTOFF){
            //printf("loop entered. logic to follow. \n");
            if(loopCount >= loopCycle){ //test function variables, substitute for time variables.
            
                mVector.magValue = kineticDrag(mVector.angValue, mVector.magValue);
                
                printf("vector magnitude: %f\n", mVector.magValue);
                printf("xVal: %f, yVal: %f\n", xVal, yVal);
                printf("Vector Init: %f; Check: %f\n", mVector.angValue, atan2(yVal, xVal));
                printf("\n");
                loopCount = 0; //
            } else { // test branch for pseudo time counter only. 
                loopCount++;
                printf("loop cycle: %d\n", loopCount);
            }
            
            //reset test. 
            if (liftTest > 1 && mVector.magValue < 3) {
                LIFTOFF = 1;
                printf("liftoff test. primary loop exit\n");
            }
        }
    }
    
    return 0;
}