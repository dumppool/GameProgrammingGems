// -----------------------------------------------------------
// KdTreeDemo - template.h
// kD-tree construction and traversal demo code
// By Jacco Bikker (July 2007)
// For Game Programming Gems
// -----------------------------------------------------------

#ifndef I_COMMON_H
#define I_COMMON_H

#define SCRWIDTH	800
#define SCRHEIGHT	600

#include "math.h"
#include "stdlib.h"
#include "emmintrin.h"
#include "stdio.h"
#include "windows.h"

inline float Rand( float a_Range ) { return ((float)rand() / RAND_MAX) * a_Range; }
int filesize( FILE* f );

typedef unsigned long Pixel;

namespace KdTreeDemo {

#ifndef __INTEL_COMPILER
#define restrict
inline float _mm_cvtss_f32(__m128 v) { float r; _mm_store_ss(&r, v); return r; }
#endif

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define _fabs	fabsf
#define _cos	cosf
#define _sin	sinf
#define _acos	acosf
#define _floor	floorf
#define _ceil	ceilf
#define _sqrt	sqrtf
#define _pow	powf
#define _exp	expf

#define CROSS(A,B)		vector3(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x)
#define DOT(A,B)		(A.x*B.x+A.y*B.y+A.z*B.z)
#define NORMALIZE(A)	{float l=1/_sqrt(A.x*A.x+A.y*A.y+A.z*A.z);A.x*=l;A.y*=l;A.z*=l;}
#define CNORMALIZE(A)	{float l=1/_sqrt(A.r*A.r+A.g*A.g+A.b*A.b);A.r*=l;A.g*=l;A.b*=l;}
#define LENGTH(A)		(_sqrt(A.x*A.x+A.y*A.y+A.z*A.z))
#define SQRLENGTH(A)	(A.x*A.x+A.y*A.y+A.z*A.z)
#define SQRDISTANCE(A,B) ((A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y)+(A.z-B.z)*(A.z-B.z))

#define PI				3.141592653589793238462f
#define EPSILON			0.00001f

class vector3
{
public:
	vector3() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {};
	vector3( float a_X, float a_Y, float a_Z ) : x( a_X ), y( a_Y ), z( a_Z ) {};
	void Set( float a_X, float a_Y, float a_Z ) { x = a_X; y = a_Y; z = a_Z; }
	void Normalize() { float l = 1.0f / Length(); x *= l; y *= l; z *= l; }
	float Length() const { return (float)sqrt( x * x + y * y + z * z ); }
	float SqrLength() const { return x * x + y * y + z * z; }
	float Dot( vector3 a_V ) const { return x * a_V.x + y * a_V.y + z * a_V.z; }
	vector3 Cross( vector3 v ) const { return vector3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ); }
	void operator += ( const vector3& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( vector3* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( const vector3& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( vector3* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( const float f ) { x *= f; y *= f; z *= f; }
	void operator *= ( const vector3& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }
	void operator *= ( vector3* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
	float& operator [] ( int a_N ) { return cell[a_N]; }
	vector3 operator- () const { return vector3( -x, -y, -z ); }
	friend vector3 operator + ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z ); }
	friend vector3 operator + ( const vector3& v1, vector3* v2 ) { return vector3( v1.x + v2->x, v1.y + v2->y, v1.z + v2->z ); }
	friend vector3 operator - ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z ); }
	friend vector3 operator - ( const vector3& v1, vector3* v2 ) { return vector3( v1.x - v2->x, v1.y - v2->y, v1.z - v2->z ); }
	friend vector3 operator - ( const vector3* v1, vector3& v2 ) { return vector3( v1->x - v2.x, v1->y - v2.y, v1->z - v2.z ); }
	// friend vector3 operator - ( const vector3* v1, vector3* v2 ) { return vector3( v1->x - v2->x, v1->y - v2->y, v1->z - v2->z ); }
	friend vector3 operator ^ ( const vector3& A, const vector3& B ) { return vector3(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x); }
	friend vector3 operator ^ ( const vector3& A, vector3* B ) { return vector3(A.y*B->z-A.z*B->y,A.z*B->x-A.x*B->z,A.x*B->y-A.y*B->x); }
	friend vector3 operator * ( const vector3& v, const float f ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	friend vector3 operator * ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x * v2.x, v1.y * v2.y, v1.z * v2.z ); }
	friend vector3 operator * ( const float f, const vector3& v ) { return vector3( v.x * f, v.y * f, v.z * f ); }
	friend vector3 operator / ( const vector3& v, const float f ) { return vector3( v.x / f, v.y / f, v.z / f ); }
	friend vector3 operator / ( const vector3& v1, const vector3& v2 ) { return vector3( v1.x / v2.x, v1.y / v2.y, v1.z / v2.z ); }
	friend vector3 operator / ( const float f, const vector3& v ) { return vector3( v.x / f, v.y / f, v.z / f ); }
	union
	{
		struct { float x, y, z; };
		struct { float cell[3]; };
	};
};

class matrix
{
public:
	enum 
	{ 
		TX=3, 
		TY=7, 
		TZ=11, 
		D0=0, D1=5, D2=10, D3=15, 
		SX=D0, SY=D1, SZ=D2, 
		W=D3 
	};
	matrix() { Identity(); }
	float& operator [] ( int a_N ) { return cell[a_N]; }
	void Identity()
	{
		cell[1] = cell[2] = cell[TX] = cell[4] = cell[6] = cell[TY] =
		cell[8] = cell[9] = cell[TZ] = cell[12] = cell[13] = cell[14] = 0;
		cell[D0] = cell[D1] = cell[D2] = cell[W] = 1;
	}
	void Init( vector3 a_Pos, float a_RX, float a_RY, float a_RZ )
	{
		matrix t;
		t.RotateX( a_RZ );
		RotateY( a_RY );
		Concatenate( t );
		t.RotateZ( a_RX );
		Concatenate( t );
		Translate( a_Pos );
	}
	void RotateX( float a_RX )
	{
		float sx = (float)sin( a_RX * PI / 180 );
		float cx = (float)cos( a_RX * PI / 180 );
		Identity();
		cell[5] = cx, cell[6] = sx, cell[9] = -sx, cell[10] = cx;
	}
	void RotateY( float a_RY )
	{
		float sy = (float)sin( a_RY * PI / 180 );
		float cy = (float)cos( a_RY * PI / 180 );
		Identity ();
		cell[0] = cy, cell[2] = -sy, cell[8] = sy, cell[10] = cy;
	}
	void RotateZ( float a_RZ )
	{
		float sz = (float)sin( a_RZ * PI / 180 );
		float cz = (float)cos( a_RZ * PI / 180 );
		Identity ();
		cell[0] = cz, cell[1] = sz, cell[4] = -sz, cell[5] = cz;
	}
	void Translate( vector3 a_Pos ) { cell[TX] += a_Pos.x; cell[TY] += a_Pos.y; cell[TZ] += a_Pos.z; }
	void SetTranslation( vector3 a_Pos ) { cell[TX] = a_Pos.x; cell[TY] = a_Pos.y; cell[TZ] = a_Pos.z; }
	void Concatenate( matrix& m2 )
	{
		matrix res;
		int c;
		for ( c = 0; c < 4; c++ ) for ( int r = 0; r < 4; r++ )
			res.cell[r * 4 + c] = cell[r * 4] * m2.cell[c] +
				  				  cell[r * 4 + 1] * m2.cell[c + 4] +
								  cell[r * 4 + 2] * m2.cell[c + 8] +
								  cell[r * 4 + 3] * m2.cell[c + 12];
		for ( c = 0; c < 16; c++ ) cell[c] = res.cell[c];
	}
	vector3 Transform( vector3& v )
	{
		float x  = cell[0] * v.x + cell[1] * v.y + cell[2] * v.z + cell[3];
		float y  = cell[4] * v.x + cell[5] * v.y + cell[6] * v.z + cell[7];
		float z  = cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11];
		return vector3( x, y, z );
	}
	void Invert()
	{
		matrix t;
		int h, i;
		float tx = -cell[3], ty = -cell[7], tz = -cell[11];
		for ( h = 0; h < 3; h++ ) for ( int v = 0; v < 3; v++ ) t.cell[h + v * 4] = cell[v + h * 4];
		for ( i = 0; i < 11; i++ ) cell[i] = t.cell[i];
		cell[3] = tx * cell[0] + ty * cell[1] + tz * cell[2];
		cell[7] = tx * cell[4] + ty * cell[5] + tz * cell[6];
		cell[11] = tx * cell[8] + ty * cell[9] + tz * cell[10];
	}
	float cell[16];
};

class aabb
{
public:
	aabb() : m_P1( vector3( 0, 0, 0 ) ), m_P2( vector3( 0, 0, 0 ) ) {};
	aabb( vector3& a_Pos, vector3& a_Size ) : m_P1( a_Pos ), m_P2( a_Pos + a_Size ) {};
	vector3& GetPos() { return m_P1; }
	vector3& GetP1() { return m_P1; }
	vector3& GetP2() { return m_P2; }
	void SetP1( vector3& a_P1 ) { m_P1 = a_P1; }
	void SetP2( vector3& a_P2 ) { m_P2 = a_P2; }
	const vector3 GetSize() const { return vector3( w(), h(), d() ); }
	bool Intersect( aabb& b2 ) const;
	bool Intersect( const vector3& C, const float r );
	bool Contains( vector3 a_Pos ) const;
	const float area() const { return 2.0f * (w() * h() + w() * d() + d() * h()); }
	const float w() const { return m_P2.x - m_P1.x; }
	const float h() const { return m_P2.y - m_P1.y; }
	const float d() const { return m_P2.z - m_P1.z; }
	const float x() const { return m_P1.x; }
	const float y() const { return m_P1.y; }
	const float z() const { return m_P1.z; }
	const float Centre( int a_Axis ) const { return (m_P1.cell[a_Axis] + m_P2.cell[a_Axis]) * 0.5f; }
	float& Min( int a_Axis ) { return m_P1.cell[a_Axis]; }
	float& Max( int a_Axis ) { return m_P2.cell[a_Axis]; }
	const int LongestSide() const;
	vector3 m_P1, m_P2;
};

}; // namespace KdTreeDemo

#endif
