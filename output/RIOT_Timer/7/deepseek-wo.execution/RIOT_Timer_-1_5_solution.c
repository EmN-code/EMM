
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
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static xtimer_t timer;
static msg_t msg_queue[8];
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 1;  // Example message type
    msg_send(&msg, thread_getpid());
    xtimer_set(&timer, TIMER_INTERVAL);
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    xtimer_init();

    timer.callback = timer_callback;
    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 1);  // Blocking call to get message from mailbox
        printf("Message received: type=%d\n", msg.type);
    }

    return 0;
}
