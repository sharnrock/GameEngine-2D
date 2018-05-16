#include "AnimatedSprite.h"
#include "AnimationEvent.h"
#include "GameObject.h"



AnimatedSprite::AnimatedSprite() :
	_frame_count(0),
	_current_frame(0),
	_frame_duration_ms(0),
	_event_taker(nullptr),
	_tick_count(0),
	_sprite_w(0),
	_sprite_h(0)
{
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::setBitmapHandle(BITMAP_HANDL bitmap_handle) 
{ 
	_bitmap_handle = bitmap_handle; 
	updateSprite(); 
}

void AnimatedSprite::setSpriteSize(int sprite_w, int sprite_h)
{
	_sprite_w = sprite_w;
	_sprite_h = sprite_h;
	updateSprite();
}

const Sprite& AnimatedSprite::generateSpriteFromCurrentFrame() const
{ 
	return _current_sprite;
}

void AnimatedSprite::updateSprite()
{
	// Only works with horizontal strip spritesheets
	int left = _current_frame * _sprite_w;
	int right = (_current_frame + 1) * _sprite_w;

	int top = 0;
	int bottom = _sprite_h;

	_current_sprite = Sprite(_bitmap_handle, left, top, right, bottom);
}

void AnimatedSprite::update(__int64 dt) // in micro seconds
{
	// Do time math
	_tick_count += dt;
	__int64 elapsed_ms = _tick_count / 1000;
	__int64 total_loop_time = _frame_count * _frame_duration_ms;
	
	int previous_frame = _current_frame;
	_current_frame = ((int)elapsed_ms / _frame_duration_ms) % _frame_count;

	if (previous_frame != _current_frame)
		updateSprite();

	if (elapsed_ms >= total_loop_time)
	{
		_tick_count -= total_loop_time;
		if (_event_taker)
			_event_taker->onEvent(&AnimationEvent(AnimationEvent::AnimationLoopEnd));
	}
}

