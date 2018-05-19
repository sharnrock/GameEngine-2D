#pragma once

#include <winerror.h>

class MainLoop
{
public:
	virtual ~MainLoop();

	virtual HRESULT initialize() = 0;
	virtual HRESULT uninitialize() = 0;
	virtual void loadLevel() = 0; // TODO: temp get rid of this
	virtual void update(__int64 dt_us) = 0;
};

