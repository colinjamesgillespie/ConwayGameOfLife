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


colour::colour(){
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
}

colour::colour(float inR,float inG,float inB){
	r = inR;
	g = inG;
	b = inB;
	
}

math2Dlib::math2Dlib(){
	
}

float math2Dlib::distance(point2D p1, point2D p2){
	return sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2));
}

float math2Dlib::fastDistance(point2D p1, point2D p2){
	return (pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2));
}

float math2Dlib::length(vec2D &v1){
	float newMag = sqrt(pow(v1.x,2)+pow(v1.y,2));
	v1.mag = newMag;
	return newMag;
}

vec2D math2Dlib::normalize(vec2D v1){
	vec2D vec;
	float len = length(v1);
	vec = vec2D(((v1.x)/len),((v1.y)/len));
	return vec;
}
vec2D math2Dlib::vectorMultiply(vec2D v1, float scalar){
	vec2D vec;
	vec = vec2D(scalar*(v1.x),scalar*(v1.y));
	return vec;
}
vec2D math2Dlib::createVector(point2D p1,point2D p2){
	vec2D vec;
	vec = vec2D(p2.x-p1.x,p2.y-p1.y);
	return vec;
}
point2D math2Dlib::movePoint(point2D p1, vec2D v1){
	point2D pt;
	pt = point2D(p1.x+v1.x,p1.y+v1.y);
	return pt;
}


