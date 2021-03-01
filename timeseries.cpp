#include "timeseries.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>


using namespace std;
/*
 * The class receives a database from a file,
 * and stores the information contained in it.
 */


/*
 * The information is taken from an Excel file
 * and saved in a map consisting of a vector title (string)
 */
TimeSeries::TimeSeries(const char *CSVfileName) {

    ifstream file;
    file.open(CSVfileName);
    string val;
    vector<float> info;
    string line, title;
    getline(file, line);
    stringstream stream(line);

    //Save the titles on the map
    while (getline(stream, title, ',')) {
        _table.insert(make_pair(title, info));
        _nameTitle.push_back(title);
    }
//Filling in the data on the map
    while (getline(file, line)) {
        int colidx = 0;
        stringstream ss(line);
        while (getline(ss, val, ',')) {
            _table.find(_nameTitle[colidx])->second.push_back(stof(val));
            colidx++;
        }
    }
}
//The function prints the data structure
void TimeSeries::print() {
    map<string, vector<float> >::iterator it;
    for (it = _table.begin(); it != _table.end(); it++) {
        cout << it->first << ":";
        for (float j : it->second) {
            cout << " [" << j << "] " << ",";
        }
        cout << endl;

    }
}

 //The function returns the data structure
map<string, vector<float>> TimeSeries::getTable() const {
    return this->_table;
}
//The function returns a column in the table
vector<float> TimeSeries::getColumn(string features) const {
    return _table.find(features)->second;
}
//The function adds a row
void TimeSeries::addRow(string line) {
    map<string, vector<float> >::iterator it;
    stringstream toAdd(line);
    string val;
    for (it = _table.begin(); it != _table.end(); it++) {
        getline(toAdd, val, ',');
        it->second.push_back(stof(val));
    }
}
//The function returns the headings (column names) in the data structure
vector<string> TimeSeries::getFeatures() const {
    return this->_nameTitle;
}
//The function returns the number of columns
int TimeSeries::numberOfColumn() const {
    return this->_nameTitle.size();
}
/*
float * TimeSeries::getArrColumn(string features) const {
    vector<float> title = getColumn(features);
    float arr[title.size()];
    for (int i = 0; i < title.size(); ++i) {


    }
    */
//The function returns the number of rows
int TimeSeries::numberOfRow() const {
    return numberOfRow(this->getFeatures()[0]);
}
//The function returns the number of rows of a specific row
int TimeSeries::numberOfRow(string features) const {
    return this->getColumn(features).size();
}
/*
vector<float> TimeSeries::getRow(int row){
    map<string, vector<float> >::iterator it;
    vector<float>line;
    for (it = _table.begin(); it != _table.end(); it++) {

        line.push_back(it->second[row-1]);

    }

    for (int i = 0; i < line.size(); ++i) {
        cout<<"check: "<<line[i]<<endl;
    }
    return line;
}
 */
//The function returns a line and its corresponding header
map<string ,float>TimeSeries::getRow(int row)const {
    map<string ,float> line;

    for (int i = 0; i < _nameTitle.size(); i++) {
        line.insert((make_pair(_nameTitle[i],_table.find(_nameTitle[i])->second[row - 1])));
    }
    return line;
}