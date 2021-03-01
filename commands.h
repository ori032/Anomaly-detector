

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include <sstream>
#include "HybridAnomalyDetector.h"

using namespace std;

typedef struct {
    TimeSeries *train;
    TimeSeries *test;
    HybridAnomalyDetector *hybrid;
    vector<AnomalyReport> anomalyReport;

} dataCommend;

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string text) = 0;

    virtual void write(float f) = 0;

    virtual void read(float *f) = 0;

    void makeFile(const string &name) {
        ofstream myFile;
        myFile.open(name);
        string line = this->read();

        while (line != "done") {
            myFile << line << endl;
            line = this->read();
        }
        myFile.close();

    }

    virtual ~DefaultIO() {}

    // you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command {
protected:
    DefaultIO *dio;
    string description;
    dataCommend *data;
public:
    Command(DefaultIO *dio) : dio(dio) {}

    virtual void execute() = 0;

    virtual void printDescription() {
        this->dio->write(this->description);
    }

    virtual ~Command() {}
};

class UploadCsv : public Command {
public:
    UploadCsv(DefaultIO *dio, dataCommend *data) : Command(dio) {
        this->description = "1.upload a time series csv file\n";
        this->data = data;
        this->data->hybrid = new HybridAnomalyDetector;

    }

    // void printDescription() override {
    //   this->dio->write(this->description);
    // }

    void execute() override {
        this->dio->write("Please upload your local train CSV file.\n");
        this->dio->makeFile("anomalyTrain.csv");
        this->data->train = new TimeSeries("anomalyTrain.csv");
        this->dio->write("Upload complete.\n");

        this->dio->write("Please upload your local test CSV file.\n");
        this->dio->makeFile("anomalyTest.csv");
        this->data->test = new TimeSeries("anomalyTest.csv");
        this->dio->write("Upload complete.\n");
    }


};


class Threshold : public Command {
public:
    Threshold(DefaultIO *dio, dataCommend *data) : Command(dio) {
        this->data = data;
        this->description = "2.algorithm settings\n";

    }

    void execute() override {
        this->dio->write("The current correlation threshold is ");
        this->dio->write(this->data->hybrid->getThreshold());
        this->dio->write("\n");
        this->dio->write("Type a new threshold\n");
        float threshold = stof(this->dio->read());
        while (0 > threshold || 1 < threshold) {
            this->dio->write("please choose a value between 0 and 1.\n");
        }
        this->data->hybrid->setThreshold(threshold);
    }

};

class Detection : public Command {

public:
    Detection(DefaultIO *dio, dataCommend *data) : Command(dio) {
        this->data = data;
        this->description = "3.detect anomalies\n";
    }

    void execute() override {
        this->data->hybrid->learnNormal(*this->data->train);
        this->data->anomalyReport = this->data->hybrid->detect(*this->data->test);
        this->dio->write("anomaly detection complete.\n");

    }

};

class Report : public Command {
public:
    Report(DefaultIO *dio, dataCommend *data) : Command(dio) {
        this->data = data;
        this->description = "4.display results\n";
    }

    void execute() override {
        for (AnomalyReport report : this->data->anomalyReport) {
            this->dio->write(report.timeStep);
            this->dio->write("         ");
            this->dio->write(report.description);
            this->dio->write("\n");
        }
        this->dio->write("Done.\n");
    }
};

class Analyze : public Command {
public:
    Analyze(DefaultIO *dio, dataCommend *data) : Command(dio) {
        this->data = data;
        this->description = "5.upload anomalies and analyze results\n";
    }

    void execute() override {
        int count=0;
        int * counter = &count;
        this->dio->write("Please upload your local anomalies file.\n");
        vector<pair<int, int>> exception = makeReport();
        vector<pair<int, int>> clientException = makeClientReport(counter);
        this->dio->write("Upload complete.\n");
        calculation(exception, clientException,*counter);


    }

    int calculation(const vector<pair<int, int>>& exception, const vector<pair<int, int>>& clientException, int count) {
       //Is the value of the server. There was a match in the client's file
       //vector<int> fn[clientException.size()];
        bool match = false;
        int negative = 0;
        float fp = 0, tp = 0;
        for (pair<int, int> server:exception) {
            for (pair<int, int> client:clientException) {
                //int i = 0;
                if (inRange(client.first, client.second, server.first, server.second)) {
                    tp++;
                    match = true;
                  //  fn->at(i)=1;
                    continue;
                }
            }
            if (!match) {
                fp++;
            }
            match = false;
        }
       // int falseNegative=0;
        //for(int i=0; i<fn->size();i++){
          //  falseNegative +=  fn->at(i);
       // }

        negative = this->data->test->numberOfRow() - count;
        float truePositive, falseAlarm;
        truePositive = tp / float(clientException.size());
        falseAlarm = fp / (float) negative;

        truePositive = floor(truePositive * 1000);
        truePositive = truePositive / 1000;

        falseAlarm = floor(falseAlarm * 1000);
        falseAlarm = falseAlarm / 1000;

        this->dio->write("True Positive Rate: ");
        this->dio->write(truePositive);
        this->dio->write("\n");

        this->dio->write("False Positive Rate: ");
        this->dio->write(falseAlarm);
        this->dio->write("\n");

    }

    bool inRange(int clientFirst, int clientSecond, int serverFirst, int serverSecond) {
        if (clientFirst <= serverFirst && serverFirst <= clientSecond ||
            clientFirst <= serverSecond && serverSecond <= clientSecond ||
            serverFirst <= clientFirst && clientFirst <= serverSecond ||
            serverFirst <= clientSecond && clientSecond <= serverSecond) {
            return true;
        }
        return false;
    }

    vector<pair<int, int>> makeReport() {
        vector<pair<int, int>> exception;
        string previousStr = this->data->anomalyReport[0].description;
        int previousI = this->data->anomalyReport[0].timeStep - 1;
        int one = previousI + 1;
        int tow;


        for (AnomalyReport report: this->data->anomalyReport) {
            if (report.timeStep != previousI + 1 || previousStr != report.description) {
                tow = previousI;
                exception.emplace_back(one, tow);
                one = report.timeStep;
                previousStr = report.description;
            }
            previousI = report.timeStep;
        }
        tow = this->data->anomalyReport[this->data->anomalyReport.size() - 1].timeStep;
        exception.emplace_back(one, tow);
        return exception;
    }

    vector<pair<int, int>> makeClientReport(int * count) {
        int one, tow, counter=0;
        string cut;
        vector<pair<int, int>> clientReport;
        string line = this->dio->read();
        while (line != "done") {
            stringstream stream(line);
            getline(stream, cut, ',');
            one = stoi(cut);
            getline(stream, cut, ',');
            tow = stoi(cut);
            clientReport.emplace_back(one, tow);
            counter += tow - one + 1;
            line = this->dio->read();
        }
        *count=counter;
        return clientReport;
    }
};

#endif /* COMMANDS_H_ */
