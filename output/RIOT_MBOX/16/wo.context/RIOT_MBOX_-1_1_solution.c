#include <xtimer.h>
#include <msg.h>
#include <stdio.h>
#include <evtimer.h>
#include <event.h>

#define MESSAGE_QUEUE_SIZE 8
#define TIMER_INTERVAL (5000000U) // Timer interval in microseconds (5 seconds)

enum {
    EVENT_TYPE = 1
};

static msg_t message_queue[MESSAGE_QUEUE_SIZE];
static evtimer_msg_event_t event;

void *message_handling_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == EVENT_TYPE) {
            printf("Event received with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    msg_init_queue(message_queue, MESSAGE_QUEUE_SIZE);

    // Initialize event timer
    evtimer_t timer;
    evtimer_init(&timer, EVTIMER_PERIODIC, NULL);

    // Configure the message event
    event.event.msg.type = EVENT_TYPE;
    event.event.msg.content.value = 42;  // Example value
    event.event.next = NULL;
    event.event.context = NULL;

    // Add the message event to the timer's event queue
    evtimer_add_msg(&timer, &event.event, TIMER_INTERVAL);

    // Start message handling thread
    kernel_pid_t pid = thread_create(
        NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        message_handling_thread, NULL, "msg_handler");

    // Run the event loop
    evtimer_run(&timer);

    return 0;
}