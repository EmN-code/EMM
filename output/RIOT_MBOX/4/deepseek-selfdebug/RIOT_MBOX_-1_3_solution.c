
#include "msg.h"
#include "thread.h"
#include "event/callback.h"
#include "event/periodic.h"
#include "ztimer.h"
#include "periph/timer.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define TIMER_DEV          TIMER_DEV(0)
#define TIMER_SPEED        (1000000U)  /* 1MHz */
#define TIMER_INTERVAL     (1000000U)  /* 1 second */

static void timer_cb(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 1;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    timer_init(TIMER_DEV, TIMER_SPEED, timer_cb, NULL);
    timer_set_periodic(TIMER_DEV, 0, TIMER_INTERVAL, TIMER_MODE_RESET_ON_MATCH);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received event from timer\n");
    }

    return 0;
}
