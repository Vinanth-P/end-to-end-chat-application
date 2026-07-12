#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
using namespace std;

class Client{

private:
    int clientSocket;
    string serverIp;
    int serverPort;

public:
    Client(string ip,int port);

    bool connectToServer();

    void run();

    void stop();

};

#endif