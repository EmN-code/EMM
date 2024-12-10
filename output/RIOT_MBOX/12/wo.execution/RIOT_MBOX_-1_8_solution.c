#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "stdio.h"
#include "evtimer.h"

#define INTERVAL (1U * XTIMER_MSEC)
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static ztimer_t timer;
static mbox_t mbox;

void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    printf("Sending timer message\n");
    mbox_put(&mbox, &msg);
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, NULL, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    static msg_t queue[MAIN_QUEUE_SIZE];
    mbox_init(&mbox, queue, MAIN_QUEUE_SIZE);
    
    ztimer_init();

    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, NULL, thread_getpid());

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message from timer\n");
    }
    
    return 0;
}