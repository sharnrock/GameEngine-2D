#pragma once
#include <Windows.h>



// I think this will only work in 64-bit Windows
class HPElapsedTimer
{
public:
	HPElapsedTimer();
	~HPElapsedTimer();

	// Call this once to set up the timer
	void start();

	// Call this repeatedly to get elapsed time from calling start or previous call of elapsed
	// returns 0 if start never called or microsends since last call
	__int64 uElapsed();

	// same as uElapsed, but returns milli seconds
	__int64 mElapsed();

private:
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;
};

