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
