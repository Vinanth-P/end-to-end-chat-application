# End-to-End Chat Application

A simple multi-threaded C++ chat application with a client and a server.

## Building the Project

Ensure you have CMake and a C++ compiler installed, then run:

```bash
mkdir -p build
cd build
cmake ..
make
```

## Running the Application

By default, both the server and client are hardcoded to use port `8080` and localhost (`127.0.0.1`).

1. **Start the Server** (in one terminal):
   ```bash
   cd build
   ./server
   ```

2. **Start the Client** (in one or more other terminals):
   ```bash
   cd build
   ./client
   ```

3. Type your messages in a client terminal and press enter to broadcast them to all other connected clients. Type `exit` to disconnect.
