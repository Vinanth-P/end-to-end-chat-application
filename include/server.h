#ifndef SERVER_H
#define SERVER_H

#include<iostream>
using namespace std;

class Server{

private:
    int port;
    int serverSocket;

public:
    Server(int port);
    bool start();
    void run();
    void stop();
    void handleClient(int clientSocket);
};

#endif