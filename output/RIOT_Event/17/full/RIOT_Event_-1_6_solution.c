#include "ztimer.h"
#include "msg.h"
#include "stdlib.h"
#include "xtimer.h"
#include "stdio.h"
#include "assert.h"
#include "string.h"
#include "unistd.h"

#define MSG_TYPE_PERIODIC (0x1234)
#define INTERVAL_SECONDS (1U)

static kernel_pid_t main_thread_pid;
static msg_t queue[8];

void *thread_event_loop(void *arg) {
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) < 1) {
            puts("Error receiving message\n");
            continue;
        }
        if (msg.type == MSG_TYPE_PERIODIC) {
            printf("Callback executed for periodic event\n");
        }
    }
    return NULL;
}

int main(void) {
    main_thread_pid = thread_getpid();
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    kernel_pid_t event_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0,
                                           thread_event_loop, NULL, "event_loop");
    assert(event_pid > KERNEL_PID_UNDEF);

    msg_t msg;
    msg.type = MSG_TYPE_PERIODIC;

    while (1) {
        msg_send(&msg, event_pid);
        xtimer_sleep(INTERVAL_SECONDS);
    }

    return 0;
}