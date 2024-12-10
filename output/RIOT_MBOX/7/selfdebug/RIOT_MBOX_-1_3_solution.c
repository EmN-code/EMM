#include <ztimer.h>
#include <stdio.h>
#include <stdbool.h>
#include <event/thread.h>
#include <event.h>
#include <xtimer.h>
#include <msg.h>

#define QUEUE_SIZE (8U)
#define TIMER_INTERVAL (1000000U)  // Set timer interval to 1 second

static msg_t queue[QUEUE_SIZE];
static event_queue_t event_queue;
static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    // Create and init event
    event_t event;
    event.type = 0x01;  // or any type you define
    // Post event to the event queue
    event_post(&event_queue, &event);
    // Reset timer
    ztimer_set(&timer, ZTIMER_USEC, TIMER_INTERVAL);
}

int main(void) {
    // Initialize message queue (mailbox)
    event_queue_init(&event_queue);

    // Initialize timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(&timer, ZTIMER_USEC, TIMER_INTERVAL);

    printf("Message box setup complete. Events are being posted every %d uSec.\n", TIMER_INTERVAL);

    while (true) {
        // Wait for and handle events
        event_t event;
        if (event_wait(&event_queue, &event)) {
            printf("Received event of type: %d\n", event.type);
        }
    }

    return 0;
}