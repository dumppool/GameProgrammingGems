#include "cameraInput.h"

#define MHI_DURATION 1

using namespace std;

/*
Default constructor.
bool start	: Specifies if the connection with the camera and the space reservations should be done at the moment of objec instantiation or later.
int v_method	: Defines which of the explained optical flow techniques must be used
	1 -> Kucas Kanade
	2 -> Image Differences
	3 -> Motion History
*/
webcamInput::webcamInput(bool start, int v_method)
{
	image=0;
	flowX = NULL;
	flowY = NULL;
	flowMOD = NULL;
	flowAUX = NULL;
	method=v_method;
	initialized=false; //this boolean prevents from multiple inicializations.
	
	if(start)
	{
		if(!init())
		{
			cout<<"error in initialization!"<<endl;
			exit(0); //if error during the inicialization -> exit.
		}
	}
	else
	{
		initialized=false; //the inicialization will be done later through a call to webcamInput::init()
	}
	kernel=cvCreateStructuringElementEx( 11, 11, 6, 6, CV_SHAPE_ELLIPSE); //Structuring element for mathematical morphology.
}

/*
This function reserves all the memory needed for the current method and initiates the camera capture.
*/
bool webcamInput::init()
{
	if(!initialized) //prevention for multiple inicializations
	{
		//inicialize camera
		if( !(capture = cvCaptureFromCAM( 0 )) ) // 0 means first webcam, -1 means any webcam
		{
			cout<<"ERROR: Camera can't be initialized."<<endl;
			return false;
		}
		
		frame = cvQueryFrame( capture ); //a sample frame is acquired.
		if( !frame ) return false; //if error: return false
		image = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 ); //create working image with the size of the frame.
		image->origin = frame->origin; //correct image orientation.
		
		//the same for mask and imageOld
		mask = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		mask->origin = frame->origin;
		imageOld = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		imageOld->origin = frame->origin;
		
		switch(method) //depending on the method
		{
		case 1: //Lucas and Kanade
			flowMOD=cvCreateImage(cvGetSize(frame),IPL_DEPTH_32F,1);
			flowMOD->origin = frame->origin;
			flowAUX=cvCreateImage(cvGetSize(frame),IPL_DEPTH_32F,1);
			flowAUX->origin = frame->origin;
			flowX=cvCreateMat(frame->height,frame->width,CV_32FC1);
			flowY=cvCreateMat(frame->height,frame->width,CV_32FC1);
			flowXX=cvCreateMat(frame->height,frame->width,CV_32FC1);
			flowYY=cvCreateMat(frame->height,frame->width,CV_32FC1);
			cvNamedWindow("lucas-kanade",1); //window where the results will be displayed.
		break;
		case 2: //image differences
			cvNamedWindow("differences",1);
		break;
		case 3: //motion history
			N=4;
			buf = (IplImage**)malloc(N*sizeof(buf[0]));
			memset( buf, 0, N*sizeof(buf[0]));
			
			for(int i=0;i<N;i++)
			{
				buf[i] = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
				cvZero( buf[i] );
			}
			last=0;
			mhi = cvCreateImage( cvGetSize(frame), IPL_DEPTH_32F, 1 );
			cvZero( mhi ); //clear MHI at the beginning
			cvNamedWindow("M_history",1); //window where the results will be displayed.
		break;
		}
		
		//SI DEBUG
		cvNamedWindow("WEBCAM",1); //window where the unmodified camera image will be displayed.
		
		initialized=true;
		return true;
	}
	else
		return false;
}

/*
Function to acquire new frames from the camera and convert them to black and white for further processing.
*/
bool webcamInput::capturarFrame()
{
	frame = cvQueryFrame( capture ); //a new image is acquired from the webcam. frame should not be deleted nor modified.
	if( !frame ) return false; //if error, return false.
	
	cvCvtColor( frame, image, CV_BGR2GRAY ); //the image is converted to graylevel and copied to image.
	return true;
}

/*
Public function to acquire new images.
*/
uchar* webcamInput::getImage() //ara es amb la imatge BW
{
	if(capturarFrame()==false) //new frame captured
	{
		return NULL;
	}
	
	cvShowImage("WEBCAM",image); //the acquired image is displayed
	return (uchar*)image->imageDataOrigin; //a pointer to the image data is returned for processing outside of the class.
}

/*
Public function to get the size of the image.
*/
void webcamInput::getSizeImage(int &w, int &h)
{
	w=frame->width;
	h=frame->height;
}

/*
This function is the main public interface: it acquires a new image and calls the appropiate method for motion determination.
*/
void webcamInput::queryFlow()
{
	getImage(); //new image is acquired
	
	switch(method)
	{
	case 1:
		lucaskanade();
	break;
	case 3:
		flowHistory();
	break;
	case 2:
		differences();
	break;
	}
	
	flowRegions(); //active pixels are counted in the selected regions
	
	cvCopy( image, imageOld ); //current image is kept in imageOld for next iteration.
	
}

/*
The image differences method for motion estimation.
*/
void webcamInput::differences()
{
	cvSmooth( image, image, CV_GAUSSIAN, 5, 5); //the image is presmoothed with a gaussian to reduce noise and improve the results.
	cvAbsDiff( imageOld, image, mask ); //current and previous images are subtracted.
	cvThreshold( mask, mask, 10, 255, CV_THRESH_BINARY ); //weak responses are filtered using a threshold.
	// /* OPTIONAL */ cvMorphologyEx(mask, mask, NULL,kernel, CV_MOP_CLOSE ,1); //if much noise is present, mathematic morphology can be used to eliminate isolated points.
	cvShowImage("differences",mask); //The result is shown.
}

/*
The Lucas and Kanade method for optical flow determination.
*/
void webcamInput::lucaskanade()
{
	cvCalcOpticalFlowLK(imageOld,image,cvSize(MIDA_OF,MIDA_OF),flowX, flowY); //the optical flow function is called.
	
	//here the modul of the flow vectors is computed to have an idea of the strength of the movement (small vectors correspond to noise.
	cvPow( flowX, flowXX, 2 );
	cvPow( flowY, flowYY, 2 );
	cvAdd( flowXX, flowYY, flowMOD );
	cvPow( flowMOD, flowMOD, 0.5 );
	
	//a threshold is applied to filter weak responses.
	cvThreshold( flowMOD, flowAUX, 10, 255, CV_THRESH_BINARY );
	
	// /* OPTIONAL */ cvMorphologyEx(flowAUX, flowAUX, NULL,kernel, CV_MOP_CLOSE ,1); //if much noise is present, mathematic morphology can be used to eliminate isolated points.
	
	//drawVectors(); //this function prints a line at every activated point of the mask image pointing to the direction of the motion vector.
	cvShowImage("lucas-kanade",flowAUX); //The result is shown.
	
}

/*
Function to print flow vectors. For debugging only.
*/
void webcamInput::drawVectors()
{
	for(int j=0;j<image->height;j++)
	{
		for(int i=0;i<image->width;i++)
		{
			if(cvGetReal2D(flowMOD, j, i )>10)
			{
				cvLine(flowAUX, cvPoint(i,j) , cvPoint(cvFloor(i+cvmGet(flowX,j,i)), cvFloor(j+cvmGet(flowY,j,i))), cvScalar(255) );
				//cout<<cvmGet(flowX,j,i)<<","<<cvmGet(flowY,j,i)<<endl;
			}
		}
	}

}

/*
The Motion History method for motion determination.
*/
void webcamInput::flowHistory()
{
	IplImage* silh;
	
	cvSmooth( image, image, CV_GAUSSIAN, 5, 5); //the image is presmoothed with a gaussian to reduce noise and improve the results.
	cvCopy( image, buf[last]); //the current image is added to the buffer.
	int idx2;
	int idx1=last;
	idx2 = (last + 1) % N; //index of (last - (N-1))th frame
	last=idx2;
	int diff_threshold=30;
	
	silh = buf[idx2];
	double timestamp = (double)clock()/CLOCKS_PER_SEC;
	
	cvAbsDiff( buf[idx1], buf[idx2], silh ); //the first and the last image of the buffer are substracted.
	
	cvThreshold( silh, silh, diff_threshold, 1, CV_THRESH_BINARY ); //a threshold is applied to filter the noise.
	cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); //the motion history image is updated with the information of the new frame.
	cvCvtScale( mhi, mask, 255./MHI_DURATION,(MHI_DURATION - timestamp)*255./MHI_DURATION ); //The 32 bits floating point image mhi is scaled to a 8 bits unsigned char image.
	
	cvShowImage("M_history",mask); //The result is shown.
}


/*
This function analyzes the images resulting from the motion detection and counts the activated pixels. Then, the values are stored in a vector, which is returned.
*/
vector<float> webcamInput::flowRegions()
{
	vector<float> sum; //vector where results will be returned.

	switch(method)
	{
	case 1:
		//In the case of the Lucas Kanade method, the result is stored in the 32 bits per pixel image flowAUX.
		for(vector<Region>::iterator i=regions.begin(); i!=regions.end(); ++i)
		{
			cvSetImageROI( flowAUX, cvRect(i->x,i->y,i->w,i->h) ); //the image Region Of Interest is set to the interest rectangle
			int k=cvCountNonZero(flowAUX); //Non-Zero pixels are counted
			sum.push_back(k/(float)(i->h*i->w)); //The result is scaled in the range [0-1]
			cvResetImageROI(flowAUX); //The image ROI is restarted.
		}
	break;
	case 2:
	case 3:
		//In the case of Image Differences or Motion History, the result is stored in the 8 bits per pixel image mask.
		for(vector<Region>::iterator i=regions.begin(); i!=regions.end(); ++i)
		{
			cvSetImageROI( mask, cvRect(i->x,i->y,i->w,i->h) ); //the image Region Of Interest is set to the interest rectangle
			int k=cvCountNonZero(mask); //Non-Zero pixels are counted
			sum.push_back(k/(float)(i->h*i->w)); //The result is scaled in the range [0-1]
			cvResetImageROI(mask); //The image ROI is restarted.
		}
	break;
	}
	return sum; //the vector is returned
}

/*
This function adds a new region to the list of interest regions.
*/
void webcamInput::addRegion(int x, int y, int w, int h)
{
	regions.push_back(Region(x,y,w,h));

}

webcamInput::~webcamInput()
{
	if(image!=0)
	{
		
		cvReleaseImage(&image);
		cvReleaseImage(&imageOld);
		cvReleaseStructuringElement( &kernel);
		cvReleaseImage(&mask);
		cvReleaseCapture(&capture);

		switch(method)
		{
		case 1://Lucas-Kanade
			cvReleaseMat(&flowX);
			cvReleaseMat(&flowY);
			cvReleaseMat(&flowXX);
			cvReleaseMat(&flowYY);
			cvReleaseImage(&flowMOD);
			cvReleaseImage(&flowAUX);
			cvDestroyWindow("lucas-kanade");
		break;
		case 2://differences
			cvDestroyWindow("differences");
		break;
		case 3://Motion History
			for(int i=0;i<N;i++)
			{
				cvReleaseImage(&(buf[i]));
			}
			cvReleaseImage(&mhi);
			free(buf);
			
			cvDestroyWindow("M_history");
		break;
		}
		
 		cvDestroyWindow("WEBCAM");
		
	}
}

