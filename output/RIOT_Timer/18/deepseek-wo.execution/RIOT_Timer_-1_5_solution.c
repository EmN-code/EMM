
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[8];
static mbox_t mbox;

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 1;  // Dummy value to indicate timer event
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void) {
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    kernel_pid_t pid = thread_getpid();
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, (void *)pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.value == 1) {
            printf("Timer triggered!\n");
            // Add your LED toggle logic here
        }
    }

    return 0;
}
