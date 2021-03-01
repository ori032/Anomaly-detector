

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures {
    string feature1, feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    float x, y;
};


class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:  vector<correlatedFeatures> cf;
protected: float threshold = 0.9;
public:
    SimpleAnomalyDetector();

    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries &ts);

    virtual float getThreshold();

    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }

    Line dOLineReg(float *x, float *y, int size, Point **p);

    bool itNew(const string &features1, const string &features2);

    float makeThreshold(Point **p, int size, Line line);

    void deleteP(Point **p, int size);

    void setThreshold(float threshold);

     virtual bool  devHelp(Point p, const correlatedFeatures &cf);
    virtual void
    learnNormalHelp(int features1, int features2, float max, float *x, float *y, vector<string> title, int size);
};


#endif /* SIMPLEANOMALYDETECTOR_H_ */