#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer.h>
#include <msg.h>
#include <stdio.h>  

#define INTERVAL      (1U * ZTIMER_MSEC)  // interval of 1 second
#define MSG_TYPE_EVENT 0x1234
#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static ztimer_t timer;

static void _callback(void *arg)
{
    (void)arg;
    static uint32_t cnt = 0;
    msg_t msg;
    msg.type = MSG_TYPE_EVENT;
    msg.content.value = cnt++;
    msg_send(&msg, sched_active_thread_pid);
}

int main(void)
{
    msg_init_queue(msg_queue, QUEUE_SIZE);

    printf("Starting evtimer_setup example\n");

    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC);
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    evtimer_msg_event_t evtimer_event;
    evtimer_event.msg.type = MSG_TYPE_EVENT;
    evtimer_event.msg.content.ptr = (void*)&timer;
    evtimer_event.event.period = INTERVAL;
    evtimer_event.event.callback = _callback;
    evtimer_event.event.arg = NULL;

    while (1) {
        msg_t msg;
        // Receive message
        msg_receive(&msg);
        printf("Received event with value: %lu\n", msg.content.value);
    }
    return 0;
}