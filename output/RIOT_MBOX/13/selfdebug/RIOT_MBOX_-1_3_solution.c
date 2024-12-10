#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <timex.h>
#include <event.h>

#define MAIN_QUEUE_SIZE     (8)
#define TIMER_INTERVAL      (1U * US_PER_SEC)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_event_t event;
    kernel_pid_t main_pid = thread_getpid();

    evtimer_init(&evtimer, EVTIMER_ONE_SHOT, NULL);
    evtimer_set(&event, TIMER_INTERVAL);

    evtimer_add(&evtimer, &event, main_pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);

        if (msg.type == EVTIMER_MSG_TYPE) {
            printf("Timer went off!\n");
            evtimer_add(&evtimer, &event, main_pid);
        }
    }

    return 0;
}