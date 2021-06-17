#ifndef ____CAMERAINPUT___H
#define ____CAMERAINPUT___H

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

#define MIDA_OF 15

/*
This class is used to define interest regions in the webcam image, where the motion will be measured.
*/
class Region
{
public:
	int x; //top-left x position.
	int y; //top-left y position.
	int w; //width of the rectangle.
	int h; //height of the rectangle.
	
	Region(int xx,int yy,int ww,int hh):x(xx),y(yy),h(hh),w(ww){}
};

/*
This class encapsules all the code to acquire images from the webcam and process them.
*/
class webcamInput
{
	public:

		webcamInput(bool start=true, int v_method=1);//1:Lucas-Kanade 2:Differences 3:Motion History
		~webcamInput();

		bool	init			( void );
		void	queryFlow		( void ); //acquire and process a new image.
		void	getSizeImage	( int &w, int &h ); //writes the width and height of the images in the parameter variables.
		uchar*	getImage		( void ); //pointer to the colour image.
		void	addRegion		( int x, int y, int w, int h ); //add a new region to the list of interest regions.

	private:
		vector<Region>	regions; //regions of interest
		bool			initialized;
		
		//----Capture of the image--------------------//
		IplImage*	frame; //Pointer to webcam frames.
		IplImage*	image; //Current image from the webcam.
		IplImage*	imageOld; //Previous image from the webcam.
		CvCapture*	capture; //Pointer to capture structure.
		
		
		//----Motion History--------------------------//
		IplImage**	buf; //Image buffer.
		int			last;
		int			N;
		int			method;
		IplImage*	mhi;
		IplImage*	mask;
		
		
		//----Optical Flow-----------------------------//
		CvMat*		flowX; //X component of the motion vectors.
		CvMat*		flowY; //Y component of the motion vectors.
		CvMat*		flowXX;
		CvMat*		flowYY;
		IplImage*	flowMOD;
		IplImage*	flowAUX;
		
		
		//-----kernel for mathematic morphology----------//
		IplConvKernel*	kernel;
	

	private:

		bool	capturarFrame	( void ); //acquire a new frame.
		void	differences		( void ); //motion detection functions.
		void	lucaskanade		( void );
		void	flowHistory		( void );
		void	drawVectors		( void ); //this function can be used to display the motion vectors in the Locas and Kanade method.
		
		vector<float>	flowRegions	( void ); //function to compute the optical flow in the regions of interest.

};

#endif // ____CAMERAINPUT___H
