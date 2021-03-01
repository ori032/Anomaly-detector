

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    void learnNormalHelp(int features1, int features2, float max, float *x, float *y,
                    vector<string> title, int size) override;

    bool  devHelp(Point p,const correlatedFeatures &cf) override;
    Point** makeArrPoint(float *x,float *y,int size);


    ~HybridAnomalyDetector() override;

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */