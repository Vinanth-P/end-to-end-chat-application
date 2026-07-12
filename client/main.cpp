#include "client.h"
using namespace std;

int main(){
    Client client("127.0.0.1",8080);

    if(!client.connectToServer()) return 1;

    client.run();
    client.stop();
    return 0;
}