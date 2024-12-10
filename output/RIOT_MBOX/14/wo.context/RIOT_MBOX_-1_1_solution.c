#include <ztimer.h>
#include <msg.h>
#include <kernel.h>
#include <evtimer_msg.h>
#include <evtimer.h>
#include <stdio.h>

#define INTERVAL_SEC (5U) // Interval for sending messages
#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)
define QUEUE_SIZE (8)

static msg_t _msg_queue[QUEUE_SIZE];
static evtimer_t _evtimer;
static evtimer_msg_event_t _evtimer_event;

void _evtimer_msg_handler(evtimer_event_t *event)
{
    evtimer_msg_event_t *evt = (evtimer_msg_event_t *)event;
    printf("Received message with content: %d\n", evt->msg.content.value);
}

void send_messages(void) {
    evtimer_set_msg(&_evtimer, ztimer_now(ZTIMER_MSEC) + INTERVAL_SEC * 1000, &_evtimer_event);
    msg_send(&_evtimer_event.msg, thread_pid_lookup("main"));
    printf("Message sent with interval of %u seconds.\n", INTERVAL_SEC);
}

int main(void)
{
    printf("Starting message box functionality on ESP32 with RIOT\n");

    msg_init_queue(_msg_queue, QUEUE_SIZE);
    evtimer_msg_event_init(&_evtimer_event);
    
    evtimer_init(&_evtimer, _evtimer_msg_handler);

    while (1) {
        send_messages();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_SEC * 1000);
    }

    return 0;
}