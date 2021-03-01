// made by yosef-natan berebi -208486365
//and ori cohen - 313363244 by 03.12.21

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "anomaly_detection_util.h"

using namespace std;


class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};

// --------------------------------------
Circle minPoints(Point **points, vector<Point> border, size_t size);

Circle minBorad(vector<Point> border);

Circle towPointsCircle(Point P1, Point P2);

Circle threePointsCircle(Point A, Point border_tow, Point C);

float midPoint(float x1, float x2);

Circle findMinCircle(Point **points, size_t size);

float dis(Point p1, Point p2);

float verticalSlope(Point A, Point B);

float getB(float slant, Point first);

float getY(float x, float b, float slant);
Point getCenter();

bool findInCircle(Point p, Circle circle);


#endif /* MINCIRCLE_H_ */
