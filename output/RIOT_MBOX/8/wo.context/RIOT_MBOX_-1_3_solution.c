#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL (1 * ZTIMER_MSEC_PER_SEC)

static ztimer_t timer;
static char timer_stack[THREAD_STACKSIZE_MAIN];

void _timer_callback(void *arg)
{
    msg_t msg;
    msg.type = 1;
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    timer.callback = _timer_callback;
    timer.arg = NULL;
    timer.long_period = false;
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == 1) {
            puts("Event received!");
            ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
        }
    }

    return 0;
}