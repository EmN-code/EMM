#include <xtimer.h>
#include <msg.h>
#include <event/timeout.h>
#include <event/callback.h>
#include <evtimer.h>

#define INTERVAL (5U * US_PER_SEC)

static void _message_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg_send(&msg, sched_pid_request(THREAD_PID_MAIN));
    puts("Message sent.");
}

int main(void)
{
    msg_t queue[8];
    msg_init_queue(queue, 8);

    evtimer_t evtimer;
    evtimer_event_t e_event;

    e_event.event.callback = _message_callback;
    e_event.event.arg = NULL;

    evtimer_init(&evtimer, evtimer_add_timeout_callback);
    evtimer_add_msg(&evtimer, &e_event, INTERVAL, THREAD_PID_MAIN);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == 0x1234) {
            puts("Message received.");
        }
    }
    return 0; 
}