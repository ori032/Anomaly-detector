
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include "Server.h"

Server::Server(int port) throw(const char *) {
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd < 0) {
        throw "Socket creation failed";
    }
    this->socServer.sin_port = htons(port);
    this->socServer.sin_addr.s_addr = INADDR_ANY;
    this->socServer.sin_family = AF_INET;

    int check = bind(this->fd, (struct sockaddr *) &this->socServer, sizeof(this->socServer));
    if (check < 0) {
        throw "Failed socket binding";
    }
    check = listen(this->fd, 10);
    if (check < 0) {
        throw "Failed socket listening";
    }


}

void Server::start(ClientHandler &ch) throw(const char *) {

    /*
    fd_set set;
    FD_ZERO(&set);
    FD_SET(this->fd,&set);

    timeval timeout;
    timeout.tv_sec=2;
    timeout.tv_usec =0;

    */

    //,set,&timeout
    this->t = new thread([this, &ch]() {
        while (!_stop) {
            /*
            int isTimeout = select(this->fd + 1,&set,0,0 ,timeout);
            if(isTimeout==0){
                close(this.)
            }
             */

           // cout << "in the loop" << endl;
           // cout << "waiting to a client" << endl;
            socklen_t clientSize = sizeof(this->socClient);
            int aClient = accept(this->fd, (struct sockaddr *) &this->socClient, &clientSize);
            if (aClient < 0) {
                throw "accept failure";
            }
          //  cout << "client connected" << endl;
            ch.handle(aClient);
            close(aClient);

        }
        close(this->fd);
    });

}

void Server::stop() {
    this->_stop = true;
    this->t->join(); // do not delete this!


}

Server::~Server() {
    delete this->t;
}

