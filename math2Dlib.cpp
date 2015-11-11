#include "math2Dlib.h"
#include <math.h>


point2D::point2D(){	
	x = 0.0f;
	y = 0.0f;
}

point2D::point2D(float inX, float inY){
	x = inX;
	y = inY;
}

vec2D::vec2D(){
	x = 0.0f;
	y = 0.0f;
	mag = 0.0f;

}

vec2D::vec2D(float inX, float inY){
	x = inX;
	y = inY;
	mag = sqrt(pow(x,2)+pow(y,2));
}

math2Dlib::math2Dlib(){
	
}




