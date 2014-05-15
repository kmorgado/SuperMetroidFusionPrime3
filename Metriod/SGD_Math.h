///////////////////////////////////////////////////////////////////////
//	File	:	"SGD_Math.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Math defines and functions.
///////////////////////////////////////////////////////////////////////

#pragma once

#define PI		3.14159265358972

//	A Single Point.
struct tPoint2D
{
	double dX, dY;				//	X and Y Coordinates.
};

//	A Direction of Movement.
struct tVector2D
{
	double dX, dY;				//	X and Y Velocities.
};

//	Finds the length of a vector (Magnitude).
double Vector2DLength(tVector2D vector);

//	Normalize a Vector.  Makes the vector a unit length of 1.
tVector2D Vector2DNormalize(tVector2D vector);

//	Rotates a vector in 2D.
//	
//	[ cos&	sin&	0 ]	  [x]	[x1]
//	[-sin&  cos&	0 ]	* [y] = [y1]
//	[  0	  0		1 ]	  [1]	[1 ]
//
//	x1 = (cos& * x)  + (sin& * y) + (0 * 1)
//  y1 = (-sin& * x) + (cos& * y) + (0 * 1)
tVector2D Vector2DRotate(tVector2D vector, double dRadians);

//	Finds the Angle between two vectors.
double AngleBetweenVectors(tVector2D &v1, tVector2D &v2);

//	Test whether or not a point is inside of a polygon.
//
//	Input	:		point			-	The point to check against.
//					pPolygon		-	The vertices in the polygon.
//					nVertexCount	-	The total number of vertices for the polygon.
//
//	Return	:	True if the point is inside of the polygon.
bool PointInPolygon(tPoint2D &point, tPoint2D *pPolygon, int nVertexCount);