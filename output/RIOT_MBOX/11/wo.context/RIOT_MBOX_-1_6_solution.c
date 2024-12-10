#include "xtimer.h"
#include "msg.h"
#include "evtimer.h"
#include <stdio.h>

#define INTERVAL_USEC (3000000U) // 3 seconds

static evtimer_t evtimer;
static evtimer_event_t event;
static mbox_t mbox;
static char msg_queue_buf[8 * sizeof(msg_t)];

static void _callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example value
    msg_send(&msg, mbox.thread_pid);
    printf("Message sent with value: %d\n", msg.content.value);
}

static void _evtimer_init(void) 
{
    evtimer_init(&evtimer, _callback);
    evtimer_request_timeout(&evtimer, INTERVAL_USEC);
    evtimer_add(&evtimer, &event);
}

int main(void)
{
    msg_init_queue((msg_t *)msg_queue_buf, sizeof(msg_queue_buf) / sizeof(msg_t));
    mbox.thread_pid = thread_getpid();
    mbox.msg_array = (msg_t *)msg_queue_buf;
    mbox.size = sizeof(msg_queue_buf) / sizeof(msg_t);

    _evtimer_init();

    while (1) {
        msg_t m;
        msg_receive(&m);
        printf("Received message with value: %d\n", m.content.value);
    }

    return 0;
}