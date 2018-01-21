#pragma once
#include "vec3.h"
#include "Scene.h"
class Mouse
{
public:
	int lastX;
	int lastY;
	void passiceMotionEvent(int x, int y)
	{
		lastX = x;
		lastY = y;
	}
	void motionEvent(int x, int y,Scene* scn)
	{
		Vec3 t;
		t[0] = x - lastX;
		t[1] = lastY-y;
		scn->translteSelect(t);
		lastX = x;
		lastY = y;
	}
};