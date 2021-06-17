#ifndef _PERFORMANCE_H_
#define _PERFORMANCE_H_

class CPerformanceCounter_Base
{
public:
	CPerformanceCounter_Base()
	{

	}

	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual float TimeInMilliseconds() = 0;
};


#ifdef _MSC_VER
	#include <windows.h>
/*
	// Performance profiling 
	LARGE_INTEGER nFrequency;
	LARGE_INTEGER nStartTime;
	LARGE_INTEGER nStopTime;
	#define START_TIME() QueryPerformanceFrequency(&nFrequency); QueryPerformanceCounter(&nStartTime);
	#define STOP_TIME() QueryPerformanceCounter(&nStopTime);
	#define TIME_IN_MILLI ((float)(nStopTime.QuadPart - nStartTime.QuadPart) / (float) nFrequency.QuadPart * 1000.0f)
*/
	class CPerformanceCounter : CPerformanceCounter_Base
	{
	public:
		CPerformanceCounter()
		{

		}

		void Start()
		{
			QueryPerformanceFrequency(&m_nFrequency); 
			QueryPerformanceCounter(&m_nStartTime);
		}

		void Stop()
		{
			QueryPerformanceCounter(&m_nStopTime);
		}

		float TimeInMilliseconds()
		{
			return ((float)(m_nStopTime.QuadPart - m_nStartTime.QuadPart) / (float) m_nFrequency.QuadPart * 1000.0f);
		}

	protected:

		LARGE_INTEGER m_nFrequency;
		LARGE_INTEGER m_nStartTime;
		LARGE_INTEGER m_nStopTime;
	};

#elif defined __GNUC__
	#include <sys/time.h>
	#include <unistd.h>
	struct timeval _tstart, _tend;
	struct timezone tz; 
	#define START_TIME() gettimeofday(&_tstart, &tz);
	#define STOP_TIME() gettimeofday(&_tend,&tz);
	#define TIME_IN_MILLI (((double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000)) - ((double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000)))
#endif

#endif
