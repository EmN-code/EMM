#include <ztimer.h>
#include <stdio.h>
#include <evtimer.h>
#include <msg.h>

#define INTERVAL (1U * ZTIMER_SEC) // Correct usage of a multiplication with integer
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];
static evtimer_t event_timer;

static void timer_callback(struct _evtimer_event *event) {
    printf("Timer event executed.\n");
}

int main(void) {
    msg_init_queue(msg_queue, QUEUE_SIZE);
    printf("Message queue initialized.\n");

    evtimer_event_t evtimer_event;
    evtimer_init(&event_timer, timer_callback, &evtimer_event);

    ztimer_sleep(ZTIMER_SEC, 2);

    evtimer_add_periodic(&event_timer, INTERVAL);  // Using the correct function for scheduling interval events
    printf("Event timer initialized and running.\n");

    return 0;
}