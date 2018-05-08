#pragma once
#include "Possessor.h"
class AIPossessor :
	public Possessor
{
public:
	AIPossessor();
	~AIPossessor();

	void update(__int64);

private:
	__int64 _dt_total;
};

