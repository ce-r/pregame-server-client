
#include "both.hpp"


int main() {

    struct sockaddr_in servaddr;
    // int port;
    
    int servfd = socket(AF_INET, SOCK_STREAM, 0);
    if (servfd < 0) {
        return NITS_SOCKET_ERROR;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port        = htons(MY_PORT);

    if (bind(servfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        close(servfd);  
        return NITS_SOCKET_ERROR;
    }

    if (listen(servfd, 5) < 0) { // 5? 
        close(servfd);  
        return NITS_SOCKET_ERROR;
    }

    global_servfd = servfd;

    // if (global_servfd == -1) {
    //     return NITS_SOCKET_ERROR;
    // }

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int acceptfd = accept(global_servfd, (struct sockaddr*) &cliaddr, &clilen);

    if (acceptfd < 0) {
        perror("error accepting connection");
        return NITS_SOCKET_ERROR;
    }

    close(acceptfd); 
    close(global_servfd);

    return 0;
}