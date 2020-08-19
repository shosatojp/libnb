#include "loop.h"
#include "nb.h"

int loop_event_count = 0;

lambda find_lambda_by_id(uint64_t id) {
    for (int i = 0; i < loop_event_count; i++) {
        if (loop_events[i].id == id) {
            return loop_events[i].fn;
        }
    }
    return NULL;
}

/**
 * return sockfd
 */
int start_server(int port) {
    int sockfd;
    struct sockaddr_in reader_addr;
    reader_addr.sin_family = PF_INET;
    reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    reader_addr.sin_port = htons(port);

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    if (bind(sockfd, (struct sockaddr*)&reader_addr, sizeof reader_addr) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(sockfd, SOMAXCONN) < 0) {
        close(sockfd);
        perror("listen");
        return -1;
    }
    return sockfd;
}

/**
 * accept sockfd
 * return new sockfd (you need to create_event)
 */
int accept_socket(int sockfd) {
    int new_sockfd;
    struct sockaddr_in writer_addr;
    int writer_len = sizeof writer_addr;
    if ((new_sockfd = accept(sockfd, (struct sockaddr*)&writer_addr, (socklen_t*)&writer_len)) < 0) {
        continue;
    }
}

int read_socket(int sockfd, void* buffer, int size) {
    read(sockfd, buffer, size);
}

// int write_socket(int sockfd)


int select_socket(){
    
}