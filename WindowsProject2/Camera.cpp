#include "Camera.h"
#include "Possessor.h"



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
	_x -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Camera::moveUp(__int64 dt)
{
	_y -= speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Camera::moveRight(__int64 dt)
{
	_x += speed_px_s * dt / 1E6f;
	updateBoundingRect();
}

void Camera::moveDown(__int64 dt)
{
	_y += speed_px_s * dt / 1E6f;
	updateBoundingRect();
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
		float offset_x = _w/2;// use own rect
		float offset_y = _h/2;

		
		float _focus_object_x_offset = _focus_object->getWidth() / 2;
		float _focus_object_y_offset = _focus_object->getHeight() / 2;

		// offset so the thing is in middle of screen
		_x = (_focus_object->X() + _focus_object_x_offset) - offset_x;
		_y = (_focus_object->Y() + _focus_object_y_offset) - offset_y;

		// gotta offset the dude too
	}
	else if (_possessor)
	{
		_possessor->update(dt);
	}
	// or nothing will happen...
}