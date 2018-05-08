#pragma once
#include "Displayable.h"
class FPSCounter :
	public DisplayableText
{
public:
	FPSCounter();
	FPSCounter(float x, float y, float w, float h);
	~FPSCounter();

	void update(__int64 dt);

private:
	int fps_count;
	long long dt_count;
};

