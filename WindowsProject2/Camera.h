#pragma once

#include "Displayable.h"
#include "Controllable.h"
class GameObject;

class Camera :
	public Displayable,
	public Controllable
{
public:
	Camera();
	~Camera();

	void moveLeft(__int64 dt);
	void moveUp(__int64 dt);
	void moveRight(__int64 dt);
	void moveDown(__int64 dt);

	void centerScreenOnThisGuy(GameObject* focus) { _focus_object = focus; }

	DrawType      getDrawType() const { return DontRender; }
	void setPossesor(Possessor* possessor); 
	void update(__int64 dt);
private:
	float speed_px_s;
	Possessor* _possessor;
	GameObject* _focus_object;
};

