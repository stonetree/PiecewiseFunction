// PiecewiseFunction.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "cPoint.h"
#include "cLine.h"

const double Rkp_star = 0.8;

int initialInputfile(vector<cPoint>& inputParameter)
{

	ifstream inputFile("input.txt");

	char x[10];
	char y[10];


	char buf[30];

	if (!inputFile)
	{
		cout<<"Unable to open input file!"<<endl;
		return -2;
	}

	while(!inputFile.eof())
	{
		inputFile.getline(buf,19,'\n');
		sscanf(buf,"%s %s\n",x, y);
		inputParameter.push_back(cPoint(atof(x),atof(y)));
	}

	return 0;
}

double calculateMean(vector<cPoint>& _pointSet, ID _startID, ID _endID)
{
	int id_index;
	double mean = 0.0;

	for (id_index = _startID;id_index <= _endID;id_index++)
	{
		mean += _pointSet[id_index].gety();
	}

	return mean/(_endID - _startID + 1);
}

double calculateSStot(vector<cPoint>& _pointSet, ID _startID, ID _endID)
{
	int id_index;
	double mean;
	double ss_tot = 0;
	
	mean = calculateMean(_pointSet,_startID,_endID);

	for (id_index = _startID;id_index <= _endID;id_index++)
	{
		ss_tot += pow(_pointSet[id_index].gety() - mean,2);
	}

	return ss_tot;
}

double calculateSSerr(vector<cPoint>& _pointSet, ID _startID, ID _endID)
{
	int id_index;
	double l_i = 0;
	double ss_err = 0;
	double y_1 = _pointSet[_startID].gety();
	double y_p = _pointSet[_endID].gety();
	double x_1 = _pointSet[_startID].getx();
	double x_p = _pointSet[_endID].getx();

	for (id_index = _startID; id_index <= _endID;id_index++)
	{
		l_i = y_1 + (y_p - y_1) * (_pointSet[id_index].getx() - x_1)/(x_p - x_1);
		ss_err += pow(_pointSet[id_index].gety() - l_i,2);
	}
	
	return ss_err;
}

double calculateRkp(vector<cPoint>& _pointSet, ID _startID, ID _endID)
{
	int id_index;
	double ss_tot;
	double ss_err;


	ss_tot = calculateSStot(_pointSet,_startID,_endID);
	ss_err = calculateSSerr(_pointSet,_startID,_endID);

	return 1 - ss_err/ss_tot;
}

double findMiniRkp(map<pair<ID,ID>,double>& _segments,ID *_startID, ID *_endID)
{
	double mini_rkp = 10000000000;
	map<pair<ID,ID>,double>::iterator iter_segments;

	for (iter_segments = _segments.begin();iter_segments != _segments.end();iter_segments++)
	{
		if (iter_segments->second < mini_rkp)
		{
			mini_rkp = iter_segments->second;
			*_startID = iter_segments->first.first;
		    *_endID = iter_segments->first.second;
		}
	}
	
	return mini_rkp;
}

void splitSubSegment(vector<cPoint>& _pointSet,ID _startID, ID _endID,map<pair<ID,ID>,double>& _segments)
{

	int id_index;
	int id_found = -1;
	double max_distance = 0;
	double tem_distance;
	double rkp_seg_1, rkp_seg_2;
	
	double l_i = 0;
	double y_1 = _pointSet[_startID].gety();
	double y_p = _pointSet[_endID].gety();
	double x_1 = _pointSet[_startID].getx();
	double x_p = _pointSet[_endID].getx();

	map<pair<ID,ID>,double>::iterator iter_segments;


	//Find the point with longest distance
	for (id_index = _startID;id_index <= _endID;id_index++)
	{
		l_i = y_1 + (y_p - y_1) * (_pointSet[id_index].getx() - x_1)/(x_p - x_1);
		tem_distance = abs(_pointSet[id_index].gety() - l_i);

		if (tem_distance>max_distance)
		{
			max_distance = tem_distance;
			id_found = id_index;
		}
	}
	
	//split the segment found
	if (id_found != -1)
	{
		cPoint point_found = cPoint(_pointSet[id_found]);
		iter_segments = _segments.find(make_pair(_startID,_endID));
		if (iter_segments == _segments.end())
		{
			cout<<"Can not find the point that should be in the segments!!!"<<endl;
			exit(1);
		}

		_segments.erase(iter_segments);

		rkp_seg_1 = calculateRkp(_pointSet,_startID,id_found);
		_segments.insert(make_pair(make_pair(_startID,id_found),rkp_seg_1));

		rkp_seg_2 = calculateRkp(_pointSet,id_found,_endID);
		_segments.insert(make_pair(make_pair(id_found,_endID),rkp_seg_2));
	}

	return;
}

void splitSegment(vector<cPoint>& _pointSet, map<pair<ID,ID>,double>& _segments)
{
	double mini_rkp;
	ID startID,endID;
	mini_rkp = findMiniRkp(_segments,&startID,&endID);

	//Check whether the minimum rkp is achieved.
	//If not, the segment that has minimum value of rkp will be split into two subsegments.
	if (mini_rkp<=Rkp_star)
	{
		splitSubSegment(_pointSet,startID,endID,_segments);
	}
		
	return;
}

void initialSegments(vector<cPoint>& _pointSet, map<pair<ID,ID>,double>& _segments)
{
	double Rkp = 0.0;

	Rkp = calculateRkp(_pointSet,0,_pointSet.size() - 1);
	_segments.insert(make_pair(make_pair(_pointSet.begin()->getID(),(_pointSet.end() - 1)->getID()),Rkp));
	return;
}


void linearRegression(vector<cPoint>& _pointSet,map<pair<ID,ID>,double>& _segments,list<cLine>& _lines)
{
	map<pair<ID,ID>,double>::iterator iter_segments;
	int iter_id,startID,endID;
	double gradient,constant;
	int num = 0;
	double sum_x,sum_y,sum_xy,sum_x2;	

	_lines.clear();

	for (iter_segments = _segments.begin();iter_segments != _segments.end();iter_segments++)
	{
		startID = _pointSet[(iter_segments->first).first].getID();
		endID = _pointSet[(iter_segments->first).second].getID();
		num = endID - startID + 1;
		sum_x = sum_y = sum_xy = sum_x2 = 0.0;


		for (iter_id = startID;iter_id <= endID;iter_id++)
		{
			sum_x += _pointSet[iter_id].getx();
			sum_x2 += pow(_pointSet[iter_id].getx(),2);
			sum_y += _pointSet[iter_id].gety();
			sum_xy += _pointSet[iter_id].getx() * _pointSet[iter_id].gety();
		}

		gradient = (num * sum_xy - sum_x * sum_y)/(num * sum_x2 - pow(sum_x,2));
		constant = (sum_x2 * sum_y - sum_x * sum_xy)/(num * sum_x2 - pow(sum_x,2));

		_lines.push_back(cLine(gradient,constant));
	}
	
	return;
}



int _tmain(int argc, _TCHAR* argv[])
{
	//store the set of initial points reading from the input file 
	vector<cPoint> pointSet;
	vector<cPoint>::iterator iter_pointSet;

	//store the lines
	list<cLine> lines;

	//Read the input file
	/*Please name the input file as "input.txt"*/
	/*and place it in the specified folder along with the source files*/
	initialInputfile(pointSet);

	//Given the set of initial points
	//move its elements in the set to make it arranged in ascending order
	sort(pointSet.begin(),pointSet.end());

	int point_id_index = 0;
	for (iter_pointSet = pointSet.begin();iter_pointSet != pointSet.end();iter_pointSet++,point_id_index++)
	{
		iter_pointSet->setID((ID)point_id_index);
	}

	//Store the segments info
	map<pair<ID,ID>,double> segments;

	//Initial segments
	//There is an initial segment starting from the first point and ending at the last point.
	initialSegments(pointSet,segments);

	splitSegment(pointSet,segments);

	linearRegression(pointSet,segments,lines);

	return 0;
}

