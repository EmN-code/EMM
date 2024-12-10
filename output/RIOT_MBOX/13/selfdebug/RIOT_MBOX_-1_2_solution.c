#include <xtimer.h>
#include <stdio.h>
#include <evtimer.h>
#include <ztimer.h>

msg_t msg;
msg_t msg_queue[8];
mbox_t mbox;
evimer_t evtimer;

enum { INTERVAL = 1000U };
static void _evtimer_callback(void *arg)
{
    printf("Event triggered\n");
    msg_t msg;
    msg.type = 0x1234;  // Example message type
    msg_send(&msg, (kernel_pid_t)arg);
}

int main(void)
{
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    mbox_init(&mbox, msg_queue, ARRAY_SIZE(msg_queue));
    evtimer_init_mbox(&evtimer);
    evtimer_msg_event_t event;
    
    event.msg.sender_pid = thread_getpid();
    evtimer.event.handler = _evtimer_callback;
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        printf("Received message\n");
    }
    return 0;
}