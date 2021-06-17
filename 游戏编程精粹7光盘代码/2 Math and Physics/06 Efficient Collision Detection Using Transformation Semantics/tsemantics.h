// Header for "Efficient Collision Detection using Transformation Semantics".

#ifndef _GPG7_TS_H___
#define _GPG7_TS_H___

// Gets the integer representation of a 32-bit floating-point
// Observe that this only works for variables and attributes.
#define GET_FIR(f)			(*(int*)(&(f)))

// <<Checks>> the sign bit from a 32-bit floating point.
#define CHECK_FLOAT_SIGN(f)	( GET_FIR(f) & 0x80000000)

// Gets the sign bit from a 32-bit floating point.
// This is used for branchless code
#define GET_FLOAT_SIGN(f)	( GET_FIR(f) >> 31 )


/// 3-D point
class Point3f
{
public:
	
	Point3f(void)
	  : x(0.0f), y(0.0f), z(0.0f)
	{
	}
	
	Point3f(float _x, float _y, float _z)
	  : x(_x), y(_y), z(_z)
	{
	}
	
	float magnitude(void) const
	{
		return sqrt( x*x + y*y + z*z);
	}
	
	void normalize(void)
	{
		float mag = x*x + y*y + z*z;

		if( mag > 0.0f )
		{
			float inv = 1.0f/sqrt(mag);
			x *= inv;
			y *= inv;
			z *= inv;
		}
	}
	
	Point3f& operator *= ( float factor )
	{
		x *= factor;
		y *= factor;
		z *= factor;
		
		return *this;
	}
	
	Point3f& operator /= ( float div )
	{
		x /= div;
		y /= div;
		z /= div;
		
		return *this;
	}
	
	float x;
	float y;
	float z;
};

/// Transformation matrix
class Matrix4f
{
public:

    Matrix4f(void){}
    Matrix4f( const Matrix4f& m )
    {
    	memcpy( a, m.a, sizeof(float)*16 );
    }
    
	void setScale( const Point3f& s )
	{
		a[ 0] *= s.x;
		a[ 1] *= s.x;
		a[ 2] *= s.x;		
		a[ 4] *= s.y;
		a[ 5] *= s.y;
		a[ 6] *= s.y;		
		a[ 8] *= s.z;
		a[ 9] *= s.z;
		a[10] *= s.z;
	}
	
	void setTrans( const Point3f& t )
	{
		a[12] = t.x;
		a[13] = t.y;
		a[14] = t.z;
	}
	
	/// implements rotation of a given angle in radians around a given axis (x,y,z)
	void rotateRad( float radians, float x, float y, float z) 
	{
		Point3f axis(x,y,z);
		float s  = sin( radians ),
			  c  = cos( radians );
		float c1 = (1-c);

		axis.normalize();		
		a[ 0] =  axis.x*axis.x*c1 + c;
		a[ 4] =  axis.x*axis.y*c1 - axis.z*s;
		a[ 8] =  axis.x*axis.z*c1 + axis.y*s;

		a[ 1] =  axis.y*axis.x*c1 + axis.z*s;
		a[ 5] =  axis.y*axis.y*c1 + c;
		a[ 9] =  axis.y*axis.z*c1 - axis.x*s;
		
		a[ 2] =  axis.z*axis.x*c1 - axis.y*s;
		a[ 6] =  axis.z*axis.y*c1 + axis.x*s;
		a[10] =  axis.z*axis.z*c1 + c;
		
		a[ 3] = a[ 7] = a[11] = a[12] = a[13] = a[14] = 0.0;
		a[15] = 1.0;
	}
	
	/// extracts semantics from this matrix (scale, rotation, translation)	
	void extractSemantics( Point3f& s, Point3f rot[3], Point3f& t ) const
	{
		// gets the translation part
		t.x = a[12];
		t.y = a[13];
		t.z = a[14];
		// gets the rotation part
		rot[0].x = a[0]; rot[0].y = a[1]; rot[0].z = a[2];
		rot[1].x = a[4]; rot[1].y = a[5]; rot[1].z = a[6];
		rot[2].x = a[8]; rot[2].y = a[9]; rot[2].z = a[10];
		// gets the scale part
		s.x = rot[0].magnitude();
		s.y = rot[1].magnitude();
		s.z = rot[2].magnitude();
		
		// normalizes the rotation part		
		rot[0] *= 1.0/s.x;
		rot[1] *= 1.0/s.y;
		rot[2] *= 1.0/s.z;
	}
	
	void inverseBySemantics( Matrix4f& out ) const
	{
		float *inv =  out.a;
		
		// 3x3 part: transpose and divides by squared scale! :P
		const float invSx2 = 1.0/(a[0]*a[0] + a[1]*a[1] + a[ 2]*a[ 2]);
		inv[0] = invSx2*a[0];
		inv[4] = invSx2*a[1];
		inv[8] = invSx2*a[2];

		const float invSy2 = 1.0/(a[4]*a[4] + a[5]*a[5] + a[ 6]*a[ 6]);
		inv[1] = invSy2*a[4];
		inv[5] = invSy2*a[5];
		inv[9] = invSy2*a[6];

		const float invSz2 = 1.0/(a[8]*a[8] + a[9]*a[9] + a[10]*a[10]);
		inv[2 ] = invSz2*a[8];
		inv[6 ] = invSz2*a[9];
		inv[10] = invSz2*a[10];

		// translation part
		inv[12] = -(inv[0]*a[12] + inv[4]*a[13] + inv[ 8]*a[14]);
		inv[13] = -(inv[1]*a[12] + inv[5]*a[13] + inv[ 9]*a[14]);
		inv[14] = -(inv[2]*a[12] + inv[6]*a[13] + inv[10]*a[14]);

		// fills last row
		inv[ 3] = inv[7] = inv[11] = 0.0;
		inv[15] = 1.0;
	}
	
	float determinant(void) const
	{
		return a[1]*a[11]*a[14]*a[4] - a[1]*a[10]*a[15]*a[4] - a[11]*a[13]*a[2]*a[4] + a[10]*a[13]*a[3]*a[4] -
				a[0]*a[11]*a[14]*a[5] + a[0]*a[10]*a[15]*a[5] + a[11]*a[12]*a[2]*a[5] - a[10]*a[12]*a[3]*a[5] -
				a[1]*a[11]*a[12]*a[6] + a[0]*a[11]*a[13]*a[6] + a[1]*a[10]*a[12]*a[7] - a[0]*a[10]*a[13]*a[7] -
				a[15]*a[2]*a[5]*a[8]  + a[14]*a[3]*a[5]*a[8]  + a[1]*a[15]*a[6]*a[8]  - a[13]*a[3]*a[6]*a[8]  -
				a[1]*a[14]*a[7]*a[8]  + a[13]*a[2]*a[7]*a[8]  + a[15]*a[2]*a[4]*a[9]  - a[14]*a[3]*a[4]*a[9]  -
				a[0]*a[15]*a[6]*a[9]  + a[12]*a[3]*a[6]*a[9]  + a[0]*a[14]*a[7]*a[9]  - a[12]*a[2]*a[7]*a[9];
	}
	
	void inverseByCofactors(Matrix4f& mat ) const
	{
		register float det = determinant();
		det = 1.0/ det;
		mat.set(
							// first row
							(-a[11]*a[14]*a[5] +a[10]*a[15]*a[5] +a[11]*a[13]*a[6] -a[10]*a[13]*a[7] -a[15]*a[6]*a[9] +a[14]*a[7]*a[9])* det,
							(a[11]*a[14]*a[4] -a[10]*a[15]*a[4] -a[11]*a[12]*a[6] +a[10]*a[12]*a[7] +a[15]*a[6]*a[8] -a[14]*a[7]*a[8])* det,
							(-a[11]*a[13]*a[4] +a[11]*a[12]*a[5] -a[15]*a[5]*a[8] +a[13]*a[7]*a[8] +a[15]*a[4]*a[9] -a[12]*a[7]*a[9])* det,
							(a[10]*a[13]*a[4] -a[10]*a[12]*a[5] +a[14]*a[5]*a[8] -a[13]*a[6]*a[8] -a[14]*a[4]*a[9] +a[12]*a[6]*a[9])*det,
							// second row
							(a[1]*a[11]*a[14] -a[1]*a[10]*a[15] -a[11]*a[13]*a[2] +a[10]*a[13]*a[3] +a[15]*a[2]*a[9] -a[14]*a[3]*a[9])*det,
							(-a[0]*a[11]*a[14] +a[0]*a[10]*a[15] +a[11]*a[12]*a[2] -a[10]*a[12]*a[3] -a[15]*a[2]*a[8] +a[14]*a[3]*a[8])*det,
							(-a[1]*a[11]*a[12] +a[0]*a[11]*a[13] +a[1]*a[15]*a[8] -a[13]*a[3]*a[8] -a[0]*a[15]*a[9] +a[12]*a[3]*a[9])*det,
							(a[1]*a[10]*a[12] -a[0]*a[10]*a[13] -a[1]*a[14]*a[8] +a[13]*a[2]*a[8] +a[0]*a[14]*a[9] -a[12]*a[2]*a[9])*det,
							// third row
							(-a[15]*a[2]*a[5] +a[14]*a[3]*a[5] +a[1]*a[15]*a[6] -a[13]*a[3]*a[6] -a[1]*a[14]*a[7] +a[13]*a[2]*a[7])*det,
							(a[15]*a[2]*a[4] -a[14]*a[3]*a[4] -a[0]*a[15]*a[6] +a[12]*a[3]*a[6] +a[0]*a[14]*a[7] -a[12]*a[2]*a[7])*det,
							(-a[1]*a[15]*a[4] +a[13]*a[3]*a[4] +a[0]*a[15]*a[5] -a[12]*a[3]*a[5] +a[1]*a[12]*a[7] -a[0]*a[13]*a[7])*det,
							(a[1]*a[14]*a[4] -a[13]*a[2]*a[4] -a[0]*a[14]*a[5] +a[12]*a[2]*a[5] -a[1]*a[12]*a[6] +a[0]*a[13]*a[6])*det,
							// fourth row
							(a[11]*a[2]*a[5] -a[10]*a[3]*a[5] -a[1]*a[11]*a[6] +a[1]*a[10]*a[7] +a[3]*a[6]*a[9] -a[2]*a[7]*a[9])*det,
							(-a[11]*a[2]*a[4] +a[10]*a[3]*a[4] +a[0]*a[11]*a[6] -a[0]*a[10]*a[7] -a[3]*a[6]*a[8] +a[2]*a[7]*a[8])*det,
							(a[1]*a[11]*a[4] -a[0]*a[11]*a[5] +a[3]*a[5]*a[8] -a[1]*a[7]*a[8] -a[3]*a[4]*a[9] +a[0]*a[7]*a[9])*det,
							(-a[1]*a[10]*a[4] +a[0]*a[10]*a[5] -a[2]*a[5]*a[8] +a[1]*a[6]*a[8] +a[2]*a[4]*a[9] -a[0]*a[6]*a[9])*det
						);
	}
	
	void set( 
				float a0, float a4, float a8,  float a12,
				float a1, float a5, float a9,  float a13,
				float a2, float a6, float a10, float a14,
				float a3 = 0.0, float a7 = 0.0, float a11 = 0.0, float a15  = 1.0
			)
	{
		a[0]=a0;	a[4]=a4;	a[8] = a8;	a[12]=a12;
		a[1]=a1;	a[5]=a5;	a[9] = a9;	a[13]=a13;
		a[2]=a2;	a[6]=a6;	a[10]=a10;	a[14]=a14;
		a[3]=a3;	a[7]=a7;	a[11]=a11;	a[15]=a15;
	}
	
	
	float operator [] ( int i ) const
    {
    	return a[i];
    }
    
    Point3f operator * ( const Point3f& v ) const
	{
		return Point3f(
						v.x * a[0] + v.y * a[4] + v.z * a[8 ] + a[12],
						v.x * a[1] + v.y * a[5] + v.z * a[9 ] + a[13],
						v.x * a[2] + v.y * a[6] + v.z * a[10] + a[14]
						);
	}

	/// Holds elements from this matrix
	float a[16];
};

/// Axis-aligned bounding box
class Aabb
{
public:

    Aabb(void)
    {
    }

	Aabb( const Point3f& maxima, const Point3f& minima )	  
	{
	   points[0] = maxima;
	   points[1] = minima;
	}
	
	void transform( const Matrix4f& M, const Aabb& box )
	{		
		// transforms each extents
        Point3f vx( M[0], M[1], M[2 ] );
		Point3f vy( M[4], M[5], M[6 ] );
		Point3f vz( M[8], M[9], M[10] );
		
		vx *= (box.points[0].x - box.points[1].x);
		vy *= (box.points[0].y - box.points[1].y);
		vz *= (box.points[0].z - box.points[1].z);

		// transform the min point
		points[0] = points[1] = M*box.points[1];

		if( CHECK_FLOAT_SIGN(vx.x) ) points[1].x += vx.x; else points[0].x += vx.x;
		if( CHECK_FLOAT_SIGN(vx.y) ) points[1].y += vx.y; else points[0].y += vx.y;
		if( CHECK_FLOAT_SIGN(vx.z) ) points[1].z += vx.z; else points[0].z += vx.z;
		if( CHECK_FLOAT_SIGN(vy.x) ) points[1].x += vy.x; else points[0].x += vy.x;
		if( CHECK_FLOAT_SIGN(vy.y) ) points[1].y += vy.y; else points[0].y += vy.y;
		if( CHECK_FLOAT_SIGN(vy.z) ) points[1].z += vy.z; else points[0].z += vy.z;
		if( CHECK_FLOAT_SIGN(vz.x) ) points[1].x += vz.x; else points[0].x += vz.x;
		if( CHECK_FLOAT_SIGN(vz.y) ) points[1].y += vz.y; else points[0].y += vz.y;
		if( CHECK_FLOAT_SIGN(vz.z) ) points[1].z += vz.z; else points[0].z += vz.z;
	}
	
	void transformNoBranches( const Matrix4f& M, const Aabb& aabb )
	{
        Point3f vx( M.a[0], M.a[1], M.a[ 2] );
		Point3f vy( M.a[4], M.a[5], M.a[ 6] );
		Point3f vz( M.a[8], M.a[9], M.a[10] );
					
		vx *= (aabb.points[0].x - aabb.points[1].x);
		vy *= (aabb.points[0].y - aabb.points[1].y);
		vz *= (aabb.points[0].z - aabb.points[1].z);

		// transforms both extreme points
		points[0] = points[1] = M*aabb.points[1];

		points[ GET_FLOAT_SIGN(vx.x) ].x += vx.x;
		points[ GET_FLOAT_SIGN(vx.y) ].y += vx.y;
		points[ GET_FLOAT_SIGN(vx.z) ].z += vx.z;					
		points[ GET_FLOAT_SIGN(vy.x) ].x += vy.x;
		points[ GET_FLOAT_SIGN(vy.y) ].y += vy.y;
		points[ GET_FLOAT_SIGN(vy.z) ].z += vy.z;					
		points[ GET_FLOAT_SIGN(vz.x) ].x += vz.x;
		points[ GET_FLOAT_SIGN(vz.y) ].y += vz.y;
		points[ GET_FLOAT_SIGN(vz.z) ].z += vz.z;
	}
		
	/// Holds the extreme points for this Aabb
	//  - maxima extreme corner is at index 0
	//  - minima extreme corner is at index 1
	Point3f points[2];
};

#endif

