// Including the necessary headers
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"

// Defining constants
#define INTERVAL (1U * US_PER_SEC)
#define STACK_SIZE THREAD_STACKSIZE_DEFAULT
#define MSG_QUEUE_SIZE (8)

// Global variables for message queue and stack
static msg_t msg_queue[MSG_QUEUE_SIZE];
static char event_stack[STACK_SIZE];

// Event thread function
void *event_thread(void *arg) {
    (void)arg;
    event_queue_t queue;
    event_t event;
    event_init(&event, EVENT_PRIO_MEDIUM, msg_init_queue(msg_queue, MSG_QUEUE_SIZE));

    while (1) {
        event_post(&queue, &event, NULL);
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create a thread for events
    thread_create(event_stack, sizeof(event_stack), THREAD_PRIORITY_MAIN - 1,
                  CREATE_STACKTEST, event_thread, NULL, "event_thread");

    // Main thread can continue executing other tasks
    while (1) {
        xtimer_sleep(INTERVAL);
    }
    return 0;
}