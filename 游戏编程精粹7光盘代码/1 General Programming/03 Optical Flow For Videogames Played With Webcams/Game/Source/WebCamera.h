#ifndef WEBCAMERA_H
#define WEBCAMERA_H

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <vector>
#include <iostream>
#include <time.h>


//-------------- DECLARACIÓN DE NUEVOS TIPOS---------------------//
#define MIDA_OF 15

class Region
{
public:
	int x; //top-left x position.
	int y; //top-left y position.
	int w; //width of the rectangle.
	int h; //height of the rectangle.
	
	Region(int xx,int yy,int ww,int hh):x(xx),y(yy),h(hh),w(ww){}
};
//---------------------------------------------------------------//



/*
This class encapsules all the code to acquire images from the webcam and process them.
*/
class WebCamera
{

public:

	//1:Lukas-Kanade, 2:Diferences, 3:M History
	WebCamera	( bool start=true, int method=1 );
	~WebCamera	( );
	
	
	void	QueryFlow			( void );

	void	GetSizeImage		( int &w, int &h );
	
	uchar*	GetImageForRender	( void );
	void	AddRegion			( int x, int y, int w, int h );
	
	void	FlowRegions			( std::vector<float> & outRegions );

	bool	IsAvailableWebCam	( void ) { return m_AvailableWebCam; }

	void	DeInit				( void );

private:

	std::vector<Region> regions;
	bool initialized;

	//Captura de la imatge
	IplImage* frame; //Pointer to webcam frames.
	IplImage* image; //Current image from the webcam.
	IplImage* imageOld; //Previous image from the webcam.
	IplImage* imageForRender;
	CvCapture *capture; //Pointer to capture structure.
	

	
	
	//motion history
	IplImage **buf; //Image buffer.
	int last;
	int N;
	int method;
	IplImage *mhi;
	IplImage *mask;
	
	//Optical Flow
	CvMat* flowX; //X component of the motion vectors.
	CvMat* flowY; //Y component of the motion vectors.
	CvMat* flowXX;
	CvMat* flowYY;
	IplImage* flowMOD;
	IplImage* flowAUX;
	
	// kernel for mathematic morphology
	IplConvKernel* kernel;
	
	bool	m_AvailableWebCam;
	
private:

	//---- PRIVATE FUNCTIONS-----
	bool	CapturarFrame	( void );
	uchar*	GetImage		( void );
	bool	Init			( void );
	

	//Funcion Optical Flow
	void	Diferencies	( void );
	void	Lukaskanade	( void );
	void	FlowHistory	( void );

	
	
};

#endif //WEBCAMERA_H


