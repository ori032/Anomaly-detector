

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <map>
#include <vector>
using namespace std;

class TimeSeries{
private:
    map<string,vector<float>> _table;
    vector<string> _nameTitle;
public:

    TimeSeries(const char* CSVfileName);
    void print();
    map<string,vector<float>> getTable()const;
    vector<float> getColumn(string features)const;
    void addRow(string line);
    vector<string>getFeatures()const;
    int numberOfColumn()const;
    int numberOfRow()const;
    int numberOfRow(string features)const;
    map<string ,float> getRow(int row)const;

    // float* getArrColumn(string features)const;


};



#endif /* TIMESERIES_H_ */