#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

// Sends a length-prefixed string over a TCP socket.
// Returns true if successful, false on error.
bool sendMessage(int socket, const std::string& msg);

// Reads a length-prefixed string from a TCP socket into out_msg.
// Returns true if successful, false on disconnect or error.
bool receiveMessage(int socket, std::string& out_msg);

#endif
