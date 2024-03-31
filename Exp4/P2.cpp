#include "pch.h"
#include "P2.h"

CP2::CP2(void)
{
	x = 0.0;
	y = 0.0;
	w = 1.0;
}


CP2::~CP2(void)
{
}

CP2::CP2(double x, double y)
{
	this->x = x;
	this->y = y;
	this->w = 1.0;
}
