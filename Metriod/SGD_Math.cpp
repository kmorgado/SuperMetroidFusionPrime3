///////////////////////////////////////////////////////////////////////
//	File	:	"SGD_Math.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Math defines and functions.
///////////////////////////////////////////////////////////////////////

#include "SGD_Math.h"

#include <math.h>
#include <float.h>

double Vector2DLength(tVector2D vector)
{
	return sqrt((vector.dX * vector.dX) + (vector.dY * vector.dY));
}

//	Normalize a Vector.
tVector2D Vector2DNormalize(tVector2D vector)
{
	tVector2D vctNormalized;

	double dLength = Vector2DLength(vector);
	vctNormalized.dX = vector.dX / dLength;
	vctNormalized.dY = vector.dY / dLength;

	//	Return the Normalized Vector.
	return vctNormalized;
}

//	Rotates a vector in 2D.
//	
//	[ cos&	sin&	0 ]	  [x]	[x1]
//	[-sin&  cos&	0 ]	* [y] = [y1]
//	[  0	  0		1 ]	  [1]	[1 ]
//
//	x1 = (cos& * x)  + (sin& * y) + (0 * 1)
//  y1 = (-sin& * x) + (cos& * y) + (0 * 1)
tVector2D Vector2DRotate(tVector2D vector, double dRadians)
{
	vector.dY *= -1;			//	For Cartesian Coordinate Conversion.

	tVector2D vctRotate;
	vctRotate.dX = ( cos(dRadians) * vector.dX) + (sin(dRadians) * vector.dY);
	vctRotate.dY = (-sin(dRadians) * vector.dX)	+ (cos(dRadians) * vector.dY);

	//	Re-apply the Conversion.  Go back into Windows Coordinate System.
	vctRotate.dY *= -1;

	//	Return the Rotate Vector.
	return vctRotate;
}

//	Finds the Angle between two vectors.
double AngleBetweenVectors(tVector2D &v1, tVector2D &v2)
{
	//	The Dot Product of the vectors.
	double dDot = (v1.dX * v2.dX) + (v1.dY * v2.dY);
	//	Length of the vectors.
	double dMag = Vector2DLength(v1) * Vector2DLength(v2);

	//	Angle between the vectors.
	double dAngle = acos(dDot / dMag);

	//	Make sure the angle is not -1.#IND00000
	//	This means that it is indefinate.  Not a number.
	if (_isnan(dAngle))
		return 0;

	//	Return the angle in radians.
	return dAngle;
}

//	Test whether or not a point is inside of a polygon.
//
//	Input	:		point			-	The point to check against.
//					pPolygon		-	The vertices in the polygon.
//					nVertexCount	-	The total number of vertices for the polygon.
//
//	Return	:	True if the point is inside of the polygon.
bool PointInPolygon(tPoint2D &point, tPoint2D *pPolygon, int nVertexCount)
{
	const double MATCH_FACTOR = 0.99f;		//	Used to coverup error in floating point.
	double dAngle = 0.0f;					//	Store the total of the angles.
	tVector2D v1, v2;						//	The vectors from point to vertex.

	//	go through all of the vertices in the polygon.
	for (int i = 0; i < nVertexCount; i++)
	{
		//	Find the first vector.
		v1.dX = pPolygon[i].dX - point.dX;
		v1.dY = pPolygon[i].dY - point.dY;

		//	Find the second vector.
		v2.dX = pPolygon[(i+1) % nVertexCount].dX - point.dX;
		v2.dY = pPolygon[(i+1) % nVertexCount].dY - point.dY;

		//	Find out what the angle was and keep adding it to
		//	see if we can get a cummulative angle of 360.
		dAngle += AngleBetweenVectors(v1, v2);
	}

	//	If the angle is 360 or greater then we are inside of the polygon.
	if (dAngle >= (MATCH_FACTOR * (2.0 * PI)))
		return true;

	//	Return Failure
	return false;
}