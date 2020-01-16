#include "nb.h"

int create_event(int __epfd, int __fd, uint32_t __events) {
    struct epoll_event _event;
    memset(&_event, 0, sizeof(struct epoll_event));
    _event.data.fd = __fd;
    _event.events = __events;
    if (epoll_ctl(__epfd, EPOLL_CTL_ADD, __fd, &_event) < 0) {
        perror("create_event > epoll_ctl");
        return -1;
    }
    return 0;
}
