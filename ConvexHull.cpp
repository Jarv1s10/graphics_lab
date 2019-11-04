#include "ConvexHull.h"

int QuickHull::findSide(mygal::Vector2<double> p1, mygal::Vector2<double> p2, mygal::Vector2<double> p)
{
	double val = (p.y - p1.y) * (p2.x - p1.x) -
		(p2.y - p1.y) * (p.x - p1.x);

	if (val > 0)
		return 1;
	if (val < 0)
		return -1;
	return 0;
}

double QuickHull::lineDist(mygal::Vector2<double> p1, mygal::Vector2<double> p2, mygal::Vector2<double> p)
{
	return abs((p.y - p1.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p.x - p1.x));
}

vector<mygal::Vector2<double>> QuickHull::quickhull(vector<mygal::Vector2<double>> a, mygal::Vector2<double> p1, mygal::Vector2<double> p2, int side)
{
	int ind = -1;
	double max_dist = 0.;

	// finding the point with maximum distance 
	// from L and also on the specified side of L. 
	for (int i = 0; i < a.size(); i++)
	{
		double temp = lineDist(p1, p2, a[i]);
		if (findSide(p1, p2, a[i]) == side && temp > max_dist)
		{
			ind = i;
			max_dist = temp;
		}
	}

	vector<mygal::Vector2<double>> res1;
	if (ind != -1)
		res1 = quickhull(a, p1, a[ind], -findSide(a[ind], p2, p1));
	if (ind != -1)
		res1.push_back(a[ind]);

	vector<mygal::Vector2<double>> res2;
	if (ind != -1)
		res2 = quickhull(a, a[ind], p2, -findSide(a[ind], p2, p1));
	for (int i = 0; i < res2.size(); ++i)
		res1.push_back(res2[i]);

	return res1;
}

vector<mygal::Vector2<double>> QuickHull::gethull(vector<mygal::Vector2<double>> a)
{
	// a[i].second -> y-coordinate of the ith point 
	if (a.size() < 3)
	{
		vector<mygal::Vector2<double>> empty;
		return empty;
	}

	// Finding the point with minimum and 
	// maximum x-coordinate 
	int min_x = 0, max_x = 0;
	for (int i = 1; i < a.size(); i++)
	{
		if (a[i].x < a[min_x].x)
			min_x = i;
		if (a[i].x > a[max_x].x)
			max_x = i;
	}

	vector<mygal::Vector2<double>> g1 = quickhull(a, a[min_x], a[max_x], 1);
	g1.push_back(a[max_x]);

	vector<mygal::Vector2<double>> g2 = quickhull(a, a[min_x], a[max_x], -1);
	reverse(begin(g2), end(g2));
	g2.push_back(a[min_x]);

	for (auto z2 : g2) {
		g1.push_back(z2);
	}
	
	return g1;
}

/////////////////////////////////////

mygal::Vector2<double> Graham::nextToTop(stack<mygal::Vector2<double>>& S)
{
	mygal::Vector2<double> p = S.top();
	S.pop();
	mygal::Vector2<double> res = S.top();
	S.push(p);
	return res;
}

void Graham::swap(mygal::Vector2<double>& p1, mygal::Vector2<double>& p2)
{
	mygal::Vector2<double> temp = p1;
	p1 = p2;
	p2 = temp;
}

double Graham::distSq(mygal::Vector2<double> p1, mygal::Vector2<double> p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

int Graham::orientation(mygal::Vector2<double> p, mygal::Vector2<double> q, mygal::Vector2<double> r)
{
	double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0.) return 0;  // colinear 
	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}

bool Graham::compare(const mygal::Vector2<double>& vp1, const mygal::Vector2<double>& vp2)
{
	
	// Find orientation 
	int o = orientation(p0, vp1, vp2);
	if (o == 0)
		return (distSq(p0, vp2) >= distSq(p0, vp1)) ? 0 : 1;

	return (o == 2) ? 0 : 1;
}

void Graham::convexHull(vector<mygal::Vector2<double>> points)
{
	int n = points.size();
	// Find the bottommost point 
	double ymin = points[0].y;
	int min = 0;
	for (int i = 1; i < n; i++)
	{
		double y = points[i].y;

		// Pick the bottom-most or chose the left 
		// most point in case of tie 
		if ((y < ymin) || (ymin == y && points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	// Place the bottom-most point at first position 
	swap(points[0], points[min]);
	// Sort n-1 points with respect to the first point. 
	// A point p1 comes before p2 in sorted output if p2 
	// has larger polar angle (in counterclockwise 
	// direction) than p1 
	p0 = points[0];
	//sort(points.begin() + 1, points.end(), compare);

	// If two or more points make same angle with p0, 
	// Remove all but the one that is farthest from p0 
	// Remember that, in above sorting, our criteria was 
	// to keep the farthest point at the end when more than 
	// one points have same angle. 
	int m = 1; // Initialize size of modified array 
	for (int i = 1; i < n; i++)
	{
		// Keep removing i while angle of i and i+1 is same 
		// with respect to p0 
		while (i < n - 1 && orientation(p0, points[i], points[i + 1]) == 0)
			i++;

		points[m] = points[i];
		m++;  // Update size of modified array 
	}

	// If modified array of points has less than 3 points, 
	// convex hull is not possible 
	if (m < 3) return;

	// Create an empty stack and push first three points 
	// to it. 

	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points 
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by 
		// points next-to-top, top, and points[i] makes 
		// a non-left turn 
		while (orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}
}

vector<mygal::Vector2<double>> Graham::gethull(vector<mygal::Vector2<double>> a)
{
	vector<mygal::Vector2<double>> res;
	while (!S.empty())
	{
		res.push_back(S.top());
	}
	reverse(begin(res), end(res));
	return res;
}

/////////////////////////////

vector<mygal::Vector2<double>> Andrew::gethull(vector<mygal::Vector2<double>> P)
{
	size_t n = P.size(), k = 0;
	if (n <= 3) return P;
	vector<mygal::Vector2<double>> H(2 * n);

	// Sort points lexicographically
	sort(P.begin(), P.end());

	// Build lower hull
	for (size_t i = 0; i < n; ++i) {
		while (k >= 2 && cross(H[k - 2], H[k - 1], P[i]) <= 0) 
			k--;
		H[k++] = P[i];
	}

	// Build upper hull
	for (size_t i = n - 1, t = k + 1; i > 0; --i) {
		while (k >= t && cross(H[k - 2], H[k - 1], P[i - 1]) <= 0) 
			k--;
		H[k++] = P[i - 1];
	}

	H.resize(k - 1);
	return H;
}


