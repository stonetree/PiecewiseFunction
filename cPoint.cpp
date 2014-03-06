#include "StdAfx.h"
#include "cPoint.h"


cPoint::cPoint(double _x,double _y, bool _isEndPoint)
{
	x = _x;
	y = _y;
	isEndPoint = _isEndPoint;
}

cPoint& cPoint::operator=(const cPoint& _point)
{
	if (this == &_point)
	{
		return *this;
	}
	
	this->id = _point.id;
	this->x = _point.x;
	this->y = _point.y;
	this->isEndPoint = _point.isEndPoint;

	return *this;
}

cPoint::cPoint(const cPoint& _point)
{
	this->operator=(_point);
}

bool cPoint::operator<(const cPoint& _point)
{
	//Here we redefine the "<" operator between two points.
	//We say point A less than point B if one of following conditions are met:
	//1) the value of x for point A is more than what point B is;
	// or 2) if it's no more than B, then the value of y will be checked,
	//and if the value of y for point A is more than what point B is, then return false,
	//otherwise return true.
	
	if (this->x > _point.x)
	{
		return false;
	}
	else
	{
		if (this->x < _point.x )
		{
			return true;
		}
		else
		{
			return this->y < _point.y ? true : false;
		}
	}
	
}

bool cPoint::operator==(const cPoint& _point)
{
	return (this->x == _point.x) && (this->y == _point.y);
}

cPoint::cPoint(void)
{
}


cPoint::~cPoint(void)
{
}
