
#include <stdio.h>
#include "xtimer.h"
#include "msg.h"

#define INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static msg_t msg_queue[8];

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
        periodic_callback(NULL);
    }

    return 0;
}
