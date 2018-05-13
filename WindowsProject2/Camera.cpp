#include "Camera.h"
#include "Possessor.h"
#include "ControlEvent.h"


Camera::Camera() :
	speed_px_s(30.0f),
	_focus_object(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::moveLeft(__int64 dt)
{
	float _x = X();
	_x -= speed_px_s * dt / 1E6f;
	setWorldCoordinates(_x, Y());
}

void Camera::moveUp(__int64 dt)
{
	float _y = Y();
	_y -= speed_px_s * dt / 1E6f;
	setWorldCoordinates(X(), _y);
}

void Camera::moveRight(__int64 dt)
{
	float _x = X();
	_x += speed_px_s * dt / 1E6f;
	setWorldCoordinates(_x, Y());
}

void Camera::moveDown(__int64 dt)
{
	float _y = Y();
	_y += speed_px_s * dt / 1E6f;
	setWorldCoordinates(X(), _y);
}

void Camera::setPossesor(Possessor* possessor)
{ 
	_possessor = possessor; 
	possessor->setControllable(this);
}

void Camera::update(__int64 dt)
{ 
	if (_focus_object)
	{
		float offset_x = getWidth() /2;// use own rect
		float offset_y = getHeight()/2;

		
		float _focus_object_x_offset = _focus_object->getWidth() / 2;
		float _focus_object_y_offset = _focus_object->getHeight() / 2;

		// offset so the thing is in middle of screen
		float _x, _y;
		_x = (_focus_object->X() + _focus_object_x_offset) - offset_x;
		_y = (_focus_object->Y() + _focus_object_y_offset) - offset_y;
		setWorldCoordinates(_x, _y);

		// gotta offset the dude too
	}
	else if (_possessor)
	{
		_possessor->update(dt);
	}
	// or nothing will happen...
}

void Camera::onControlEvent(ControlEvent* e)
{
	switch (e->getControl())
	{
	case ControlEvent::MoveDown:
		moveDown(e->getDt());
		break;
	case ControlEvent::MoveUp:
		moveUp(e->getDt());
		break;
	case ControlEvent::MoveLeft:
		moveLeft(e->getDt());
		break;
	case ControlEvent::MoveRight:
		moveRight(e->getDt());
		break;
	}
}