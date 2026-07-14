#include "../include/protocol.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <iostream>

using namespace std;

// Helper to send exactly N bytes
static bool sendAll(int socket, const char* buffer, size_t length) {
    size_t totalSent = 0;
    while (totalSent < length) {
        ssize_t bytesSent = send(socket, buffer + totalSent, length - totalSent, 0);
        if (bytesSent <= 0) return false;
        totalSent += bytesSent;
    }
    return true;
}

// Helper to receive exactly N bytes
static bool recvAll(int socket, char* buffer, size_t length) {
    size_t totalReceived = 0;
    while (totalReceived < length) {
        ssize_t bytesReceived = recv(socket, buffer + totalReceived, length - totalReceived, 0);
        if (bytesReceived <= 0) return false;
        totalReceived += bytesReceived;
    }
    return true;
}

bool sendMessage(int socket, const string& msg) {
    uint32_t length = msg.length();
    uint32_t networkLength = htonl(length);

    // Send 4-byte header
    if (!sendAll(socket, (const char*)&networkLength, sizeof(networkLength))) {
        return false;
    }

    // Send payload
    if (length > 0) {
        if (!sendAll(socket, msg.c_str(), length)) {
            return false;
        }
    }
    return true;
}

bool receiveMessage(int socket, string& out_msg) {
    uint32_t networkLength = 0;

    // Read 4-byte header
    if (!recvAll(socket, (char*)&networkLength, sizeof(networkLength))) {
        return false;
    }

    uint32_t length = ntohl(networkLength);
    
    // Prevent massive allocations on bad data
    if (length > 10 * 1024 * 1024) { // 10MB limit for sanity
        return false;
    }

    if (length == 0) {
        out_msg = "";
        return true;
    }

    // Read payload
    vector<char> buffer(length);
    if (!recvAll(socket, buffer.data(), length)) {
        return false;
    }

    out_msg.assign(buffer.data(), length);
    return true;
}
