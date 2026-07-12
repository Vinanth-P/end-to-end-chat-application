#include <iostream>
#include "server.h"

int main() {
    Server server(8080);

    if(!server.start()){
        return 1;
    }
    server.run();
    server.stop();
    return 0;
}
