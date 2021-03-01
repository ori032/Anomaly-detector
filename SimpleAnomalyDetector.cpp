
#include "SimpleAnomalyDetector.h"
#include <iterator>
#include <math.h>
#include <iostream>
#include <vector>

/*
 * The department discovers anomalies and reports them
 */
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

/*
 * The function learns the data and the correlation between the data
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    vector<string> title = ts.getFeatures();
    int numFeatures = ts.numberOfColumn();
    int size = ts.numberOfRow();
    int features1 = 0, features2 = 0;
    float num = 0.0, max = 0.0;
    vector<float> column_i;
    vector<float> column_j;

//The function goes through every 2 columns
// and fits each column, the column with the highest collection
    for (int i = 0; i < numFeatures; ++i) {
        vector<float> column_i = ts.getColumn(title[i]);
        max = 0;
        //float *x = &ts.getColumn(title[i])[0];
        float *x = column_i.data();
        float *y;
        for (int j = i+1; j < numFeatures; ++j) {
            column_j = ts.getColumn(title[j]);
            y = column_j.data();
            num = fabs(pearson(x, y, size));
            if (max < num) {
                max = num;
                features1 = min(i, j);
                features2 = std::max(i, j);
            }

        }
        column_j = ts.getColumn(title[features2]);
        y = column_j.data();

//The function checks that the correlation is at the threshold, and keeps the variables
        //if (max >= 0.9 && itNew(title[features1], title[features2])) {
        learnNormalHelp(features1, features2, max, x, y, title, size);
    }

}
//If the condition occurs, the function fills the data
 void
SimpleAnomalyDetector:: learnNormalHelp(int i, int j, float max, float *x, float *y, vector<string> title, int size) {
    if (max >= this->threshold) {
        correlatedFeatures correl;
        correl.feature1 = title[i];
        correl.feature2 = title[j];
        correl.corrlation = max;
        auto **p = new Point *[size];
        correl.lin_reg = dOLineReg(x, y, size, p);
        correl.threshold = makeThreshold(p, size, correl.lin_reg);
        cf.push_back(correl);
        // cout << correl.feature1 << " " << correl.feature2 << endl;
        deleteP(p, size);
    }
}



//The function takes 2 arrays and creates an array of points from them
Line SimpleAnomalyDetector::dOLineReg(float *x, float *y, int size, Point **p) {
    // p = new Point *[size];
    for (int i = 0; i < size; ++i) {
        p[i] = new Point(x[i], y[i]);
    }
    return linear_reg(p, size);
}

//The function checks if the correlation already exists
bool SimpleAnomalyDetector::itNew(const string &features1, const string &features2) {
    for (const correlatedFeatures &cor : cf) {
        if (cor.feature1 == features1 && cor.feature2 == features2)
            return false;
    }
    return true;
}

//The function checks what is the biggest anomaly (threshold)
float SimpleAnomalyDetector::makeThreshold(Point **p, int size, Line line) {
    float num, max = 0;
    for (int i = 0; i < size; ++i) {
        num = dev(*p[i], line);
        if (num > max) {
            max = num;
        }
    }
    return 1.1 * max;
}

//The function obtains a data structure, scans it, and reports anomalies
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> report;
    for (const correlatedFeatures &correl :cf) {
        vector<float> feature1 = ts.getColumn(correl.feature1);
        vector<float> feature2 = ts.getColumn(correl.feature2);
        for (int i = 0; i < ts.numberOfRow(); ++i) {
            float x = feature1[i];
            float y = feature2[i];
            Point p(x, y);
            if (devHelp(p, correl)) {
                report.emplace_back(AnomalyReport(correl.feature1 + "-" + correl.feature2, i + 1));
            }
        }
    }
    return report;
}
//If there was a deviation, the function returns true
bool SimpleAnomalyDetector::devHelp(Point p, const correlatedFeatures &correl) {
    float deviation = dev(p, correl.lin_reg);
    if (deviation >= correl.threshold) {
        return true;
    }
    return false;
}

//The function frees the memory in the arr of points
void SimpleAnomalyDetector::deleteP(Point **p, int size) {
    for (int i = 0; i < size; ++i) {
        delete p[i];
    }
}

void SimpleAnomalyDetector::setThreshold(float threshold) {
    this->threshold = threshold;
}
float SimpleAnomalyDetector::getThreshold() {
    return this->threshold;
}
