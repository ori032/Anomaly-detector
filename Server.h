/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <netinet/in.h>
#include <thread>
#include "CLI.h"


using namespace std;

class SocketIO : public DefaultIO {
    ifstream in;
    ofstream out;
    int clientFD;
public:
    explicit SocketIO(int fd) {
        this->clientFD = fd;
    }

    string read() override {
        string buffer = "";
        char c = 0;
        recv(clientFD, &c, sizeof(char), 0);
        while (c != '\n') {
            buffer += c;
            recv(this->clientFD, &c, sizeof(char), 0);
        }
        return buffer;

    }

    void write(string text) override {
        send(this->clientFD, text.c_str(), text.size(), 0);
        // write(serverFD,"\n",1);

    }

    void write(float f) override {
        stringstream input;
        input<<f;
        string output =input.str();
        string a = to_string(f).c_str();
        send(this->clientFD, output.c_str(), output.size(), 0);
    }

    void read(float *f) override {
        string buffer = "";
        char c = 0;
        recv(clientFD, &c, sizeof(char), 0);
        while (c != '\n') {
            buffer += c;
            recv(this->clientFD, &c, sizeof(char), 0);
        }
        *f = stof(buffer);
    }
};


// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    void handle(int clientID) override {
        SocketIO io(clientID);
        CLI cli(&io);
        cli.start();
        // CLI cli;


    }
};


// implement on Server.cpp
class Server {
    thread *t; // the thread to run the start() method in
    int fd;  //the file destructor
    sockaddr_in socServer;
    sockaddr_in socClient;
    bool _stop = false;

    // you may add data members

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
