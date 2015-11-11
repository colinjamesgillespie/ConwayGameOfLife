#ifndef MATH2D_H_INCLUDED
#define MATH2D_H_INCLUDED

class point2D{
public:
	point2D();
	point2D(float inX, float inY);
	float x;
	float y;
};

class vec2D{
public:
	vec2D();
	vec2D(float inX, float inY);
	float x; 
	float y;
	float mag;
};

class colour{
public:
	colour();
	colour(float inR,float inG,float inB);
	float r; //red
	float g; //green
	float b; //blue

};

class math2Dlib{
public:
	math2Dlib();
	static float distance(point2D p1, point2D p2);
	static float fastDistance(point2D p1, point2D p2);
	static float length(vec2D &v1);
	static vec2D normalize(vec2D v1);
	static vec2D vectorMultiply(vec2D v1, float scalar);
	static vec2D createVector(point2D p1,point2D p2);
	static point2D movePoint(point2D, vec2D);
};



#endif

