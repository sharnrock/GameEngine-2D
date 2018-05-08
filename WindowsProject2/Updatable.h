#pragma once
class Updatable
{
public:
	virtual ~Updatable();
	virtual void update(__int64 dt) = 0;
};

