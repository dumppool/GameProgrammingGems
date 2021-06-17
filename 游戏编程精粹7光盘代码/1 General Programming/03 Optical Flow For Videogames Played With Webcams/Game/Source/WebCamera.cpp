#include "WebCamera.h"

#define MHI_DURATION 1
/*
Default constructor.
bool start	: Specifies if the connection with the camera and the space reservations should be done at the moment of objec instantiation or later.
int v_method	: Defines which of the explained optical flow techniques must be used
	1 -> Kucas Kanade
	2 -> Image Differences
	3 -> Motion History
*/
WebCamera::WebCamera(bool start, int v_method)
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
		if (!Init())
		{
			m_AvailableWebCam = false;
			return;
		}
		else
		{
			m_AvailableWebCam = true;
		}

	}
	else
	{
		initialized=false; //the inicialization will be done later through a call to webcamInput::init()
	}
		
	kernel=cvCreateStructuringElementEx( 11, 11, 6, 6, CV_SHAPE_ELLIPSE); //Structuring element for mathematical morphology.
}

WebCamera::~WebCamera()
{
}

void WebCamera::DeInit( void )
{
	if(image!=0)
	{
		
		cvReleaseImage(&image);
		//cvReleaseImage(&imageForRender);
		cvReleaseImage(&imageOld);
		cvReleaseStructuringElement( &kernel);
		cvReleaseImage(&mask);
		cvReleaseCapture(&capture);

		switch(method)
		{
			case 1://Lukas-Kanade
				{
					cvReleaseMat(&flowX);
					cvReleaseMat(&flowY);
					cvReleaseMat(&flowXX);
					cvReleaseMat(&flowYY);
					cvReleaseImage(&flowMOD);
					cvReleaseImage(&flowAUX);
				}
				break;
			case 2://diferencies
				break;
			case 3://Motion History
				{
					for(int i=0;i<N;i++)
					{
						cvReleaseImage(&(buf[i]));
					}
					cvReleaseImage(&mhi);
					free(buf);
				}
				break;
		}//END switch(method)

	}//END if(image!=0)
}

/*
This function reserves all the memory needed for the current method and initiates the camera capture.
*/
bool WebCamera::Init( void )
{
	if(!initialized) //prevention for multiple inicializations
	{
		//inicialize camera
		if( !(capture = cvCaptureFromCAM( 0 )) ) // 0 means first webcam, -1 means any webcam
		{
			return  false;
		}
		
		///A sample frame is acquired.
		frame = cvQueryFrame( capture );
		if( !frame )
		{
			return false;
		}

		imageForRender = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 3 );
		imageForRender->origin = frame->origin;
		image = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		image->origin = frame->origin;
		
		
		//the same for mask and imageOld
		mask = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		mask->origin = frame->origin;
		imageOld = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		imageOld->origin = frame->origin;
		
		switch(method) //depending on the method
		{
		case 1: // Lukas-Kanade
			{
				flowMOD=cvCreateImage(cvGetSize(frame),IPL_DEPTH_32F,1);
				flowMOD->origin = frame->origin;
				flowAUX=cvCreateImage(cvGetSize(frame),IPL_DEPTH_32F,1);
				flowAUX->origin = frame->origin;
				flowX=cvCreateMat(frame->height,frame->width,CV_32FC1);
				flowY=cvCreateMat(frame->height,frame->width,CV_32FC1);
				flowXX=cvCreateMat(frame->height,frame->width,CV_32FC1);
				flowYY=cvCreateMat(frame->height,frame->width,CV_32FC1);
			}
			break;
		case 2: //image differences
			break;
		case 3: //motion history
			{
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
			}
			break;
		}
		
		initialized=true;
		return true;
	}
	else
		return false;
}

/*
Function to acquire new frames from the camera and convert them to black and white for further processing.
*/
bool WebCamera::CapturarFrame()
{
	bool flag = true;
	frame = cvQueryFrame( capture ); //a new image is acquired from the webcam. frame should not be deleted nor modified.
	cvFlip(frame,frame,0);
	cvFlip(frame,frame,1);
	imageForRender = frame;
		
	if( !frame )  //if error, return false.
	{
		flag = false;
	}
	
	cvCvtColor( frame, image, CV_BGR2GRAY );
	return flag;
}

/*
The image differences method for motion estimation.
*/
void WebCamera::Diferencies()
{
	cvSmooth( image, image, CV_GAUSSIAN, 5, 5); //the image is presmoothed with a gaussian to reduce noise and improve the results.
	cvAbsDiff( imageOld, image, mask ); //current and previous images are subtracted.
	cvThreshold( mask, mask, 10, 255, CV_THRESH_BINARY ); //weak responses are filtered using a threshold.
	// /* OPTIONAL */ cvMorphologyEx(mask, mask, NULL,kernel, CV_MOP_CLOSE ,1); //if much noise is present, mathematic morphology can be used to eliminate isolated points.	
}

/*
The Lucas and Kanade method for optical flow determination.
*/
void WebCamera::Lukaskanade()
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
}


/*
The Motion History method for motion determination.
*/	
void WebCamera::FlowHistory()
{
	//FLOW HISTORY
	
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
}


/*
This function is the main public interface: it acquires a new image and calls the appropiate method for motion determination.
*/
void WebCamera::QueryFlow()
{
	GetImage();

	// BUSCAR FLOW AMB ALGUN METODE
	switch(method)
	{
	case 1:
		Lukaskanade();
	break;
	case 3:
		FlowHistory();
	break;
	case 2:
		Diferencies();
	break;
	}
	
	//A partir de la variable mask mirar el flow a les diferents regions
	std::vector<float> dummy;
	FlowRegions(dummy);
	
	cvCopy( image, imageOld ); //current image is kept in imageOld for next iteration.
	
}

/*
This function analyzes the images resulting from the motion detection and counts the activated pixels. Then, the values are stored in a vector, which is returned.
*/
void WebCamera::FlowRegions( std::vector<float> & outRegions )
{
	//mask
	std::vector<Region>::iterator it;
	switch(method)
	{
	case 1:
		//In the case of the Lucas Kanade method, the result is stored in the 32 bits per pixel image flowAUX.
		for(it=regions.begin(); it!=regions.end(); ++it)
		{
			cvSetImageROI( flowAUX, cvRect(it->x, it->y, it->w, it->h) ); //the image Region Of Interest is set to the interest rectangle
			int k=cvCountNonZero(flowAUX); //Non-Zero pixels are counted
			outRegions.push_back(k/(float)(it->h * it->w)); //The result is scaled in the range [0-1]
			cvResetImageROI(flowAUX); //The image ROI is restarted.
		}
	break;
	case 2:
	case 3:
		//In the case of Image Differences or Motion History, the result is stored in the 8 bits per pixel image mask.
		for(it=regions.begin(); it!=regions.end(); ++it)
		{
			cvSetImageROI( mask, cvRect(it->x, it->y, it->w, it->h) ); //the image Region Of Interest is set to the interest rectangle
			int k=cvCountNonZero(mask); //Non-Zero pixels are counted
			outRegions.push_back(k/(float)(it->h * it->w)); //The result is scaled in the range [0-1]
			cvResetImageROI(mask); //The image ROI is restarted.
		}
	break;
	}
}

/*
Public function to acquire new images.
*/
uchar* WebCamera::GetImage( void ) 
{
	if(CapturarFrame() == false) //new frame captured
	{
		return NULL;
	}
	
	return (uchar*)image->imageDataOrigin;
}


uchar* WebCamera::GetImageForRender( void ) 
{
	CapturarFrame();
	
	return (uchar*)imageForRender->imageDataOrigin;
}

/*
Public function to get the size of the image.
*/
void WebCamera::GetSizeImage(int &w, int &h)
{
	w=frame->width;
	h=frame->height;
}

/*
This function adds a new region to the list of interest regions.
*/
void WebCamera::AddRegion(int x, int y, int w, int h)
{
	regions.push_back(Region(x,y,w,h));
}
