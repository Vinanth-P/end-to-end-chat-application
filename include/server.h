#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <mutex>
#include <vector>
#include <unordered_map>
using namespace std;

class Server{

private:
    int port;
    int serverSocket;
    vector<int> clientSockets;
    unordered_map<int, string> clientNames;
    mutex clientMux;

public:
    Server(int port);
    bool start();
    void run();
    void stop();
    void handleClient(int clientSocket);
    void broadcast(int senderFd, const string &msg);
};

#endif