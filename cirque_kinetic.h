/*

*/

//friction, needs grav constant. a = gu
const float grav = 9.82;
const float PI = 3.14159;
const float rad2deg = 180/PI;

float xVal = 30; //will be int in the final op, currently a test value. Will be equal to x delta.
float yVal = 40; //will be int in the final op, currently a test value. Will be equal to y delta. 

typedef struct {
    int xPoint;
    int yPoint;
    float magValue;
    float angValue;
} mouseThings;

//Function Declarations
float kineticDrag (float vecAngle, float vecMagn);
void kineticVector (int xMouse, int yMouse);
