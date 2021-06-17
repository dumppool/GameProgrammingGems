#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "cameraInput.h"
#include <time.h>

using namespace std;

int main(int argc, char *argv[])
{
	clock_t inici,fi;
	
	webcamInput webcam(true,1); // 1:Lucas-Kanade 2:Differences 3:Motion History

	int w,h;
	webcam.getSizeImage(w,h);
	cout<<"width:"<<w<<" height:"<<h<<endl;

	while(1)
	{
		inici=clock();
		webcam.queryFlow();
		//cout<<i<<" no"<<endl;
		if(cvWaitKey(10)==27)
			break;
		fi=clock();
		
		cout<<((fi-inici)/(double)CLOCKS_PER_SEC)-0.01<<endl;
 	}
	
  return EXIT_SUCCESS;
}

