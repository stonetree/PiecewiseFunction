#include "StdAfx.h"
#include "cLine.h"


cLine::cLine(void)
{
}


cLine::~cLine(void)
{
}

cLine::cLine(double _gradient,double _constant)
{
	gradient = _gradient;
	constant = _constant;
}

cLine& cLine::operator=(cLine& _line)
{
	if (this == &_line)
	{
		return *this;
	}

	gradient = _line.gradient;
	constant = _line.constant;

	startPoint = _line.startPoint;
	endPoint = _line.endPoint;

	return *this;
}

bool cLine::operator==(const cLine& _line)
{
	return (this->startPoint == _line.startPoint) && (this->endPoint == _line.endPoint);
}

cLine::cLine(cLine& _line)
{
	this->operator=(_line);
}
