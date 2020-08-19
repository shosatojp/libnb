#include "nb.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "loop.h"

uint64_t tick = 0;

int main() {
    int epfd;
    if ((epfd = epoll_create(MAX_EVENTS)) < 0) {
        fprintf(stderr, "epoll_create()\n");
        exit(1);
    }

    create_event(epfd, open("mp.c", O_RDONLY | O_NONBLOCK), EPOLLIN);
    create_event(epfd, open("hoge.md", O_CREAT | O_RDWR | O_NONBLOCK), EPOLLOUT);
    create_event(epfd, start_server(8092), EPOLLIN);

    struct epoll_event events[MAX_EVENTS];
    struct tick_event tick_events[MAX_EVENTS];
    int tick_events_count = 0;

    while (1) {
        // IO Event (wait 1ms)
        int nfd;
        if ((nfd = epoll_wait(epfd, events, MAX_EVENTS, 1)) < 0) {
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfd; i++) {
            lambda fn;
            if (fn = find_lambda_by_id(events[i].data.u64)) {
                fn();
                // id->eventオブジェクトを検索
                // 関数を入れといてそれを呼ぶ
                
            }
        }

        // Time based
        for (int i = 0; i < tick_events_count; i++) {
            if (tick_events[i].tick < tick) {
                tick_events[i].fn();
            }
        }

        tick++;
    }

    while (1) {
        int nfd;
        if ((nfd = epoll_wait(epfd, events, MAX_EVENTS, EP_TIMEOOUT)) < 0) {
            perror("epoll_wait");
            exit(1);
        }
        // printf(".");
        for (int i = 0; i < nfd; i++) {
            struct epoll_event event = events[i];
            
            if (event.data.fd == sockfd && event.events & EPOLLIN) {  // accept
                int sockfd = events[i].data.fd;
                int new_sockfd;
                struct sockaddr_in writer_addr;
                int writer_len = sizeof writer_addr;
                if ((new_sockfd = accept(sockfd, (struct sockaddr*)&writer_addr, (socklen_t*)&writer_len)) < 0) {
                    continue;
                }

                create_event(epfd, new_sockfd, EPOLLIN);
            } else if (event.events & EPOLLIN) {  // read
                char buf[1000] = {0};
                read(event.data.fd, buf, 999);
                printf("%s\n\n", buf);
                fflush(stdout);

                epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
                create_event(epfd, event.data.fd, EPOLLOUT);
            } else if (event.events & EPOLLOUT) {  // write
                const char* str = "HTTP/1.1 200 OK=\r\ncontent-length:6\r\n\r\nhoge\r\n";
                write(event.data.fd, str, strlen(str));
                epoll_ctl(epfd, EPOLL_CTL_DEL, event.data.fd, NULL);
                close(event.data.fd);
            }
        }
    }
}