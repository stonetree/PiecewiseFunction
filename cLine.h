#pragma once

#include "cPoint.h"


class cLine
{
private:
	double gradient;
	double constant;

	cPoint startPoint;
	cPoint endPoint;


public:
	void setGradient(double _gradient){gradient = _gradient;}
	double getGradient(void) const {return gradient;}
	void setConstant(double _constant){constant = _constant;}
	double getConstant(void) const {return constant;}
	
	void setStartPoint(const cPoint& _point){startPoint = _point;}
	cPoint getStartPoint(void) const {return startPoint;}
	void setEndPoint(const cPoint& _point){endPoint = _point;}
	cPoint getEndPoint(void) const {return endPoint;}

public:
	bool operator==(const cLine& _line);


public:
	cLine(void);
	cLine(double _gradient,double _constant);
	
	cLine(cLine& _line);
	cLine& operator=(cLine& _line);
	~cLine(void);
};

