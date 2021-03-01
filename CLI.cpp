#include "CLI.h"

CLI::CLI(DefaultIO *dio) {
    this->data = new dataCommend;
    this->dio = dio;
    auto *uploadCsv = new UploadCsv(this->dio, data);
    this->commend.push_back(uploadCsv);
    auto *threshold = new Threshold(this->dio, data);
    this->commend.push_back(threshold);

    auto *detect = new Detection(this->dio, data);
    this->commend.push_back(detect);

    auto *report = new Report(this->dio, data);
    this->commend.push_back(report);

    auto *analyze = new Analyze(this->dio, data);
    this->commend.push_back(analyze);



    //this->data->hybrid = new HybridAnomalyDetector;
}

void CLI::start() {
    //   UploadCsv uploadCsv = UploadCsv(this->dio);
    int i = 0;
    do {
        this->dio->write("Welcome to the Anomaly Detection Server.\n");
        this->dio->write("Please choose an option:\n");
        for (Command *command:this->commend) {
            command->printDescription();
        }
        this->dio->write("6.exit\n");

        i = stoi(this->dio->read()) - 1;
        if (0 <= i && i < 5)
            this->commend[i]->execute();
    } while (i != 5);
}


CLI::~CLI() {
    delete this->data->test;
    delete this->data->train;
    delete this->data->hybrid;
    delete this->data;
}

