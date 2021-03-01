
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}
//If the condition occurs, the function fills the data
void HybridAnomalyDetector::learnNormalHelp(int features1, int features2, float max, float *x, float *y,
                                            vector<string> title, int size) {
    //Check if it belongs to SimpleAnomalyDetector
    if (max >= this->threshold) {
        SimpleAnomalyDetector::learnNormalHelp(features1, features2, max, x, y, title, size);

        //Check if it belongs to the circuit
    } else if (max >= 0.5) {
        correlatedFeatures correl;

        Point **p = makeArrPoint(x, y, size);

        float deviation = 1.1;
        Circle circle = findMinCircle(p, size);
        correl.threshold = circle.radius * deviation;
        correl.x = circle.center.x;
        correl.y = circle.center.y;
        correl.feature1 = title[features1];
        correl.feature2 = title[features2];
        correl.corrlation = max;
        cf.push_back(correl);
        deleteP(p, size);

    }

}
//The function checks which case the situation is suitable for.
// If the usual case occurred she sends to Dad.
// Otherwise if there was a deviation, the function returns true
bool HybridAnomalyDetector::devHelp(Point toCheck, const correlatedFeatures &cf) {
    if (cf.corrlation >= this->threshold) {
        return SimpleAnomalyDetector::devHelp(toCheck, cf);
    }
    Point pointCircle = Point(cf.x, cf.y);
    Circle circle(pointCircle, cf.threshold);
    return !findInCircle(toCheck, circle);

}

Point **HybridAnomalyDetector::makeArrPoint(float *x, float *y, int size) {
    auto **p = new Point *[size];

    for (int i = 0; i < size; ++i) {
        p[i] = new Point(x[i], y[i]);

    }
    return p;
}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
