
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <iomanip>
#include <algorithm>


#include "tsemantics.h"
using namespace std;


int main( int argc, char **argv )
{	
	const Aabb box( Point3f(-1,-1,-1), Point3f(1, 1, 1) );	
	const Point3f scale( 0.72f, 3.15f, 4.908f );
	const Point3f trans( -12.3f, 256.3f, 414.1f);
		
	Point3f axes[3];

	Matrix4f M;
	
	const float angle = 0.5678f;
	
	// builds a rotation matrix and gets the axis
	M.rotateRad( angle, 3, 2, 1 );	
		axes[0].x = M.a[ 0]; 
		axes[0].y = M.a[ 1];
		axes[0].z = M.a[ 2];	
		axes[1].x = M.a[ 4];
		axes[1].y = M.a[ 5];
		axes[1].z = M.a[ 6];	
		axes[2].x = M.a[ 8];
		axes[2].y = M.a[ 9];
		axes[2].z = M.a[10];
	
	// adds the scaling part
	M.setScale( scale );
	M.setTrans( trans );
	
	// 1) Transformation semantics part.
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "- Original semantics " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << " scale: " << scale.x << ", " << scale.y << ", " << scale.z << std::endl;	
	std::cout << "     X: " << axes[0].x << ", " << axes[0].y << ", " << axes[0].z << std::endl;
	std::cout << "     Y: " << axes[1].x << ", " << axes[1].y << ", " << axes[1].z << std::endl;
	std::cout << "     Z: " << axes[2].x << ", " << axes[2].y << ", " << axes[2].z << std::endl;
	std::cout << " trans: " << trans.x << ", " << trans.y << ", " << trans.z << std::endl;	
	
	// semantics extraction
	Point3f myScale;
	Point3f myAxes[3];
	Point3f myTrans;
	
	M.extractSemantics( myScale, myAxes, myTrans );	
	
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "- Computed semantics " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "scale': " << myScale.x << ", " << myScale.y << ", " << myScale.z << std::endl;	
	std::cout << "    X': " << myAxes[0].x << ", " << myAxes[0].y << ", " << myAxes[0].z << std::endl;
	std::cout << "    Y': " << myAxes[1].x << ", " << myAxes[1].y << ", " << myAxes[1].z << std::endl;
	std::cout << "    Z': " << myAxes[2].x << ", " << myAxes[2].y << ", " << myAxes[2].z << std::endl;
	std::cout << "trans': " << myTrans.x << ", " << myTrans.y << ", " << myTrans.z << std::endl;
	
	

	// 2) transformation of Axis-aligned bounding boxes
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "- Axis-aligned bounding box transformation " << std::endl;
	
    Aabb brute;
   
	Point3f corners[8] = {
						   M*Point3f( box.points[0].x, box.points[0].y, box.points[0].z ),
						   M*Point3f( box.points[0].x, box.points[0].y, box.points[1].z ),
						   M*Point3f( box.points[0].x, box.points[1].y, box.points[0].z ),
						   M*Point3f( box.points[0].x, box.points[1].y, box.points[1].z ),
						   M*Point3f( box.points[1].x, box.points[0].y, box.points[0].z ),
						   M*Point3f( box.points[1].x, box.points[0].y, box.points[1].z ),
						   M*Point3f( box.points[1].x, box.points[1].y, box.points[0].z ),
						   M*Point3f( box.points[1].x, box.points[1].y, box.points[1].z ) };
	brute.points[0] = brute.points[1] = corners[0];
	for( int k=1;k<8;k++ )
	{
		brute.points[1].x = std::min( corners[k].x, brute.points[1].x );
		brute.points[1].y = std::min( corners[k].y, brute.points[1].y );
		brute.points[1].z = std::min( corners[k].z, brute.points[1].z );
		
		brute.points[0].x = std::max( corners[k].x, brute.points[0].x );
		brute.points[0].y = std::max( corners[k].y, brute.points[0].y );
		brute.points[0].z = std::max( corners[k].z, brute.points[0].z );
	}
    
    std::cout << "--------------------------------------------------------------------" << std::endl;
    std::cout << "  brute-force: " << std::endl;
	std::cout << "  " << brute.points[1].x << ", " << brute.points[1].y << ", " << brute.points[1].z << std::endl;
	std::cout << "  " << brute.points[0].x << ", " << brute.points[0].y << ", " << brute.points[0].z << std::endl;
    
	Aabb branchy;
	
	branchy.transform( M, box );
	
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "with branches: " << std::endl;
	std::cout << "  " << branchy.points[1].x << ", " << branchy.points[1].y << ", " << branchy.points[1].z << std::endl;
	std::cout << "  " << branchy.points[0].x << ", " << branchy.points[0].y << ", " << branchy.points[0].z << std::endl;
	

	Aabb branchless;
	branchless.transformNoBranches( M, box );	
		
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "   branchless: " << std::endl;
	std::cout << "  " << branchless.points[1].x << ", " << branchless.points[1].y << ", " << branchless.points[1].z << std::endl;
	std::cout << "  " << branchless.points[0].x << ", " << branchless.points[0].y << ", " << branchless.points[0].z << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	
	// 3) matrix inversion
	Matrix4f co;
	M.inverseByCofactors( co );	
	
	struct PrintMatrix
	{
		PrintMatrix( Matrix4f& mat )
		{
			std::cout << setiosflags(ios::fixed) << setprecision(4) << mat[0] << "\t"  << mat[4] << "\t"  << mat[ 8] << "\t"  << mat[12]  << std::endl;
			std::cout << setiosflags(ios::fixed) << setprecision(4) << mat[1] << "\t"  << mat[5] << "\t"  << mat[ 9] << "\t"  << mat[13]  << std::endl;
			std::cout << setiosflags(ios::fixed) << setprecision(4) << mat[2] << "\t"  << mat[6] << "\t"  << mat[10] << "\t"  << mat[14]  << std::endl;
			std::cout << setiosflags(ios::fixed) << setprecision(4) << mat[3] << "\t"  << mat[7] << "\t"  << mat[11] << "\t"  << mat[15]  << std::endl;
		}
	};
	
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "- Inverse by co-factors " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	PrintMatrix invco(co);	
	
	Matrix4f ts;	
	M.inverseBySemantics( ts );
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "- Inverse by semantics " << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	PrintMatrix invts(ts);
	
			

	return 1;
}

