#ifndef CLOCK_H
#define CLOCK_H

//-----------------------------------------------------------------------------
// Class Clock
//-----------------------------------------------------------------------------
class Clock
{
public:
       Clock();
       ~Clock();

       double getElapsedTime();
       double getTime();

       void sleep(const double t);
       void reset(double time = 0);

private:
       double m_start;
       double m_base;
};


#endif
 
