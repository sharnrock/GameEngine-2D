#pragma once
#include "Sprite.h"
class AnimatedSprite //:
	//public Sprite
{
public:
	AnimatedSprite();
	~AnimatedSprite();

	int getFrameCount() { return _frame_count; }
	void setFrameCount(int fc) { _frame_count = fc; }
	void incrementFrame() { ++_current_frame; }
	void decrementFrame() { --_current_frame; }
	int getCurrentFrame() { return _current_frame; }

protected:
	int _frame_count;
	int _current_frame;
};

