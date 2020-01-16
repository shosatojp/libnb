#pragma once
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_EVENTS 100
#define EP_TIMEOOUT 10000

typedef void (*lambda)();

struct tick_event {
    uint64_t tick;
    lambda fn;
};

enum event_type {
    WRITE = 1 << 0,
    READ = 1 << 1,
    FILE_IO = 1 << 2,
    SOCK_SERVER = 1 << 3,
    SOCK_CLIENT = 1 << 4
};

struct loop_event {
    int id;
    lambda fn;
    enum event_type event_type;
};

int create_event(int __epfd, int __fd, uint32_t __events);


struct loop_event loop_events[MAX_EVENTS];
int loop_event_count;
lambda find_lambda_by_id(uint64_t id);