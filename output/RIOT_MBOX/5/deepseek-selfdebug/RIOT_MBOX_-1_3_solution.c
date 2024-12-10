
#include "thread.h"
#include "msg.h"
#include "event.h"
#include "ztimer.h"
#include "mbox.h"

#define MAIN_QUEUE_SIZE     (8)
#define EVENT_INTERVAL      (1000000U)  /* 1 second */

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static mbox_t _mbox;
static msg_t _mbox_queue[MAIN_QUEUE_SIZE];

static void _event_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

static void _event_timer_callback(void *arg)
{
    event_t event;
    event.handler = _event_handler;
    event.next = NULL;
    event.list_node.next = NULL;
    event.list_node.prev = NULL;

    mbox_put(&_mbox, &event);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&_mbox, _mbox_queue, MAIN_QUEUE_SIZE);

    ztimer_t event_timer;
    ztimer_set(ZTIMER_USEC, &event_timer, EVENT_INTERVAL, _event_timer_callback, NULL);

    while (1) {
        msg_t msg;
        mbox_get(&_mbox, &msg);
        event_t *event = (event_t *)msg.content.ptr;
        event->handler(event->arg);
    }

    return 0;
}
