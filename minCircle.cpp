//
// Created by yosi on 29.12.2020.
//

#include "minCircle.h"

//distance function between 2 points.
float dis(Point p1, Point p2) {
    return sqrt((pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2)));
}

//return the mid of value of tow points.
float midPoint(float x1, float x2) {
    return (x1 + x2) / 2;
}

//return the vertical slope of a slope between tow points.
float verticalSlope(Point A, Point B) {
    return -1 * (1 / ((B.y - A.y) / (B.x - A.x)));
}

//return the value of the free member b in the equation.
float getB(float slant, Point first) {
    return first.y - (slant * first.x);
}

//same about y.
float getY(float x, float b, float slant) {
    return ((slant * x) + b);
}

//return if the point is inside the circle-mining the dis is smaller than the radius.
bool findInCircle(Point p, Circle circle) {
    float my_dis = dis(p, circle.center);
    return my_dis <= circle.radius;
}

//the main function that we run - created a vector of point and return the min circle.
Circle findMinCircle(Point **points, size_t size) {
    vector<Point> border;
    return minPoints(points, border, size);
}

/*
 * this function is the recursive algorithm - checking if the size of the points is 0,
 * or the vector with the points is equal to 3,so we can stop - because we can know which center we need -
 * according 3 points on the border of the circle - by the sentence "all the middle verticals of a triangle
 * are meeting in the center of the circle - meaning our circle that we looking for.
 * so the algo will be - erase one of the points from the list and keep it,run the function again with one point less -
 * when we back from the calling check if the point is inside the circle - if it's ok - keep going back,
 * if not - add the point to the list of border - that mean the point is one that we are looking for.
 * and than run the function again with the point inside the vector of the border.
 */
Circle minPoints(Point **points, vector<Point> border, size_t size) {
    //stop condition
    if ((size == 0) || (border.size() == 3)) {
        return minBorad(border);
    }
    //keep one of the points - that in the middle.
    Point point = Point(points[size - 1]->x, points[size - 1]->y);
    //call the function recursive
    Circle min_circle = minPoints(points, border, size - 1);
    //checking if the point in inside the circle
    if (findInCircle(point, min_circle)) {
        return min_circle;
    }
    //add the point to the list and call the function again.
    border.push_back(point);
    return minPoints(points, border, size - 1);
}

//stop condition - check how much points we have in the border vector (the one we initialize in the begin.)
//according to the size - return the min circle.
Circle minBorad(vector<Point> border) {
    if (border.empty()) {
        return {Point(0, 0), 0};
    }
    if (border.size() == 1) {
        return {border[0], 0};
    }
    //return circle by 2 points.
    if (border.size() == 2) {
        return towPointsCircle(border[0], border[1]);
    }
    //3 points
    return threePointsCircle(border[0], border[1], border[2]);
}

// take tow point - find the middle that will be the center,and find the radius with dis between them.
Circle towPointsCircle(Point P1, Point P2) {
    float x = midPoint(P1.x, P2.x);
    float y = midPoint(P1.y, P2.y);
    Point center(x, y);
    float radius = (dis(P1, P2)) / 2;
    return {center, radius};

}

/*
 * this function create a circle base on 3 points only -
 * find the tow lines that create a triangle
 * find the middle of the lines - and the vertical line of him
 * find the intersection point of the tow lines and that will be the center.
 */
Circle threePointsCircle(Point A, Point B, Point C) {
    //mid point of A and B and the slope of them.
    Point mid_ab(midPoint(A.x, B.x), midPoint(A.y, B.y));
    float slope_ab = verticalSlope(A, B);
    //same about B and C
    Point mid_bc(midPoint(B.x, C.x), midPoint(B.y, C.y));
    float slope_bc = verticalSlope(B, C);
    //find the free members of the lines.
    float b = getB(slope_ab, mid_ab);
    float c = getB(slope_bc, mid_bc);
    //find the x and the y of the intersection point.
    float x = (c - b) / (slope_ab - slope_bc);
    float y = getY(x, b, slope_ab);
    Point center = Point(x, y);
    float radius = dis(center, A);
    return {center, radius};
}