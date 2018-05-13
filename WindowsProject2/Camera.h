#pragma once

#include "Displayable.h"
class GameObject;
class ControlEvent;
class Possessor;

class Camera :
	public Displayable
{
public:
	Camera();
	~Camera();

	void centerScreenOnThisGuy(GameObject* focus) { _focus_object = focus; }

	DrawType getDrawType() const { return DontRender; }
	void     setPossesor(Possessor* possessor); 
	void     update(__int64 dt);

protected:
	void onControlEvent(ControlEvent* e);
	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);

private:
	float       speed_px_s;
	Possessor*  _possessor;
	GameObject* _focus_object;
};

