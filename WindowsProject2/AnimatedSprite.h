#pragma once

#include "Sprite.h"
#include "GString.h"
#include "Types.h"

class GameObject;

class AnimatedSprite
{
public:
	AnimatedSprite();
	~AnimatedSprite();

	int  getFrameCount() const   { return _frame_count; }
	void setFrameCount(int fc)   { _frame_count = fc; }
	void incrementFrame()        { ++_current_frame; }
	void decrementFrame()        { --_current_frame; }
	void seekFrame(int frame)    { _current_frame = frame; }
	int  getCurrentFrame() const { return _current_frame; }

	void setFrameDuration(int frame_duraction_ms) { _frame_duration_ms = frame_duraction_ms; }

	void setBitmapHandle(BITMAP_HANDL bitmap_handle);
	void setSpriteSize(int sprite_w, int sprite_h);
	
	void sendEventsToGameObject(GameObject* event_taker) { _event_taker = event_taker; }

	// TODO: rename this to just get current sprite or something..
	const Sprite& generateSpriteFromCurrentFrame() const;
	void update(__int64 dt);

protected:
	// updates the sprite info everytime the frame is advanced
	void updateSprite();

	int          _frame_count;
	int          _current_frame;
	int          _frame_duration_ms;

	GameObject*  _event_taker;

	__int64      _tick_count; // holds us from timer
	BITMAP_HANDL _bitmap_handle;
	int          _sprite_w;
	int          _sprite_h;
	Sprite       _current_sprite;
};

