#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <evtimer.h>

#define TIMER_INTERVAL (ZTIMER_MSEC_TO_TICKS(1000))
#define QUEUE_SIZE (8)

static char _stack[THREAD_STACKSIZE_MAIN];
static msg_t _queue[QUEUE_SIZE];

static evtimer_t _evtimer;

static void _event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 123; // Example value
    printf("Sending message with value: %lu\n", msg.content.value);
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    msg_init_queue(_queue, QUEUE_SIZE);

    // Set up evtimer and its queue
    evtimer_event_t timer_event;
    evtimer_event_init(&timer_event, &_event_handler, NULL);
    evtimer_add(&_evtimer, &timer_event, TIMER_INTERVAL);

    printf("Starting event timer with interval %lu ms\n", ZTIMER_USEC_DIV_DELTA(TIMER_INTERVAL, 1000));

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with value: %lu\n", msg.content.value);
    }

    return 0;
}