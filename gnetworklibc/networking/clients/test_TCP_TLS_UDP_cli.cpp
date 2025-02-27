
#include "TTU_Client.hpp"

int main() {
    gnetwork::TTU_Client client(AF_INET, SOCK_STREAM, 0, 4443, INADDR_LOOPBACK);
    client.launch();
    return 0;
}
