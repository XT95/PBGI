#ifdef __WIN32__
    #include <windows.h>
#endif
#ifdef __linux__
    #include <unistd.h>
    #include <sys/time.h>
#endif

#include "Utils/clock.h"

//-----------------------------------------------------------------------------
// Constructor / Destructor
//-----------------------------------------------------------------------------
Clock::Clock()
{
    reset();
    m_base = 0.;
    m_base = getTime();
}

Clock::~Clock()
{

}

//-----------------------------------------------------------------------------
// Get functions
//-----------------------------------------------------------------------------
double Clock::getElapsedTime()
{
    return getTime() - m_start;
}

double Clock::getTime()
{
    #ifdef __linux__
        timeval Time = {0, 0};
        gettimeofday(&Time, NULL);
        return (Time.tv_sec + Time.tv_usec / 1000000.);
    #endif
    #ifdef __WIN32__
		static bool isInitialized;
		static double timerFreq;
		static unsigned __int64 tmp;
		if ( !isInitialized ) {
			isInitialized = true;
			QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);
			timerFreq = 1.0/(double)tmp;
		}
		QueryPerformanceCounter((LARGE_INTEGER*)&tmp);	
        return ((double)tmp) * timerFreq-m_base;
    #endif
}

//-----------------------------------------------------------------------------
// Other functions
//-----------------------------------------------------------------------------
void Clock::sleep(const double t)
{
    #ifdef __linux__
        usleep((unsigned int)t*1000000);
    #endif
    #ifdef __WIN32__
        ::Sleep(static_cast<DWORD>(t * 1000));
    #endif
}

void Clock::reset(double time)
{
    m_start = getTime()-time;
}
