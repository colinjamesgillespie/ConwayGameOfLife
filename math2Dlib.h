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


class math2Dlib{
public:
	math2Dlib();
	
};



#endif

