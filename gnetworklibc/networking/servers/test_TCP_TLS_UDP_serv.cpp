
#include "TTU_Server.hpp"

int main() {
    gnetwork::TTU_Server server("server.crt", "server.key", AF_INET, SOCK_STREAM, 0, 4443, INADDR_ANY, 10);
    server.launch();
    return 0;
}