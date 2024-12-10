#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <string.h>

#define INTERVAL   (1U * US_PER_SEC)
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    msg_t msg;
    ztimer_tick_t last_wakeup = ztimer_now(ZTIMER_USEC);

    while (1) {
        ztimer_periodic_wakeup(ZTIMER_USEC, &last_wakeup, INTERVAL);
        printf("Sending event message\n");

        msg.content.value = 42;  // Example event content
        msg_send(&msg, sched_active_pid);
    }
    return 0;
}