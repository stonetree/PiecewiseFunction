#pragma once
class cPoint
{
private:
	ID id;
	double x;
	double y;
	bool isEndPoint;

public:
	void setID(ID _id){id = _id;}
	ID   getID(void) const {return id;}

	void setx(double _x){x = _x;}
	double getx(void)const {return x;}

	void sety(double _y){y = _y;}
	double gety(void) const {return y;}

	void setIsEndPoint(bool _isEndPoint){isEndPoint = _isEndPoint;}
	bool getIsEndPoint(void) const {return isEndPoint;}

public:
	bool operator<(const cPoint& _point);
	bool operator==(const cPoint& _point);

public:
	cPoint(double _x,double _y, bool _isEndPoint = false);
	cPoint(const cPoint& _point);
	cPoint& operator=(const cPoint& _point);
	cPoint(void);
	~cPoint(void);
};

