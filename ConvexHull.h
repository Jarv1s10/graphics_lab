#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include <stack>
#include <stdlib.h>
#include <set>
#include <iostream>
#include "Vector2.h"

using namespace std;

class QuickHull
{
public:
	vector<mygal::Vector2<double>> gethull(vector<mygal::Vector2<double>> a);
private:
	set<mygal::Vector2<double>> hull;
	int findSide(mygal::Vector2<double> p1, mygal::Vector2<double> p2, mygal::Vector2<double> p);
	double lineDist(mygal::Vector2<double> p1, mygal::Vector2<double> p2, mygal::Vector2<double> p);
	vector<mygal::Vector2<double>> quickhull(vector<mygal::Vector2<double>> a, mygal::Vector2<double> p1, mygal::Vector2<double> p2, int side);
};


class Graham
{
public:
	vector<mygal::Vector2<double>> gethull(vector<mygal::Vector2<double>> a);
private:

	stack<mygal::Vector2<double>> S;
	mygal::Vector2<double> p0;
	mygal::Vector2<double> nextToTop(stack<mygal::Vector2<double>>& S);
	bool compare(const mygal::Vector2<double>& vp1, const mygal::Vector2<double>& vp2);
	void swap(mygal::Vector2<double>& p1, mygal::Vector2<double>& p2);
	double distSq(mygal::Vector2<double> p1, mygal::Vector2<double> p2);
	int orientation(mygal::Vector2<double> p, mygal::Vector2<double> q, mygal::Vector2<double> r);
	
	void convexHull(vector<mygal::Vector2<double>> points);
};


class Andrew
{
public:
	vector<mygal::Vector2<double>> gethull(vector<mygal::Vector2<double>> P);
private:
	double cross(const mygal::Vector2<double>& O, const mygal::Vector2<double>& A, const mygal::Vector2<double>& B)
	{
		return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
	}
};




