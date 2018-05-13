#include "HPElapsedTimer.h"

HPElapsedTimer::HPElapsedTimer()
{
	Frequency.QuadPart = 0;
}

HPElapsedTimer::~HPElapsedTimer()
{
}

__int64 HPElapsedTimer::uElapsed()
{
	if (Frequency.QuadPart == 0)
		return 0;

	LARGE_INTEGER EndingTime, ElapsedMicroseconds;

	QueryPerformanceCounter(&EndingTime);

	// find tick difference
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	// Set up next elapsed measurement
	StartingTime = EndingTime;

	// convert to micro seconds
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	
	// return the 64bit portion of the union
	return ElapsedMicroseconds.QuadPart;
}

__int64 HPElapsedTimer::mElapsed()
{
	if (Frequency.QuadPart == 0)
		return 0;

	LARGE_INTEGER EndingTime, ElapsedMicroseconds;

	QueryPerformanceCounter(&EndingTime);

	// find tick difference
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	// Set up next elapsed measurement
	StartingTime = EndingTime;

	// convert to milli seconds
	ElapsedMicroseconds.QuadPart *= 1000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	// return the 64bit portion of the union
	return ElapsedMicroseconds.QuadPart;
}

void HPElapsedTimer::start()
{
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
}