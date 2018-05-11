#include "FPSCounter.h"



FPSCounter::FPSCounter()
{
}


FPSCounter::~FPSCounter()
{
}

FPSCounter::FPSCounter(float x, float y, float w, float h) :
	DisplayableText(x, y, w, h)
{
	content = "hello again";
	if (content == GString("hello again"))
		OutputDebugString(TEXT("It worked"));
	OutputDebugString(TEXT("It didn't work"));
}

void FPSCounter::update(__int64 dt)
{
	++fps_count;
	dt_count += dt;
	if (dt_count > 500000)
	{
		content = GString::number(fps_count*2);
		dt_count = 0;
		fps_count = 0;
	}
}