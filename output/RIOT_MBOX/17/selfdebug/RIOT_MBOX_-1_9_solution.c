#include <xtimer.h>  // For xtimer functionalities
#include <thread.h>  // For thread functionalities
#include <msg.h>     // For message passing functionalities
#include <stdio.h>    // For standard input/output

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void *message_thread(void *arg) {
    (void)arg;
    msg_t msg;
    evtimer_event_t event;
    evtimer_t timer;

    // Initialize event timer object
    evtimer_init(&timer, &event);

    // Set up the event timer
    uint32_t interval = 1000000; // 1 second interval
    evtimer_add(&timer, &event, interval);
    
    while (1) {
        // Wait indefinitely for a message
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);

        // Re-add the event timer if necessary
        evtimer_add(&timer, &event, interval);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    char message_stack[THREAD_STACKSIZE_DEFAULT];

    kernel_pid_t thread_pid = thread_create(
        message_stack, sizeof(message_stack),
        THREAD_PRIORITY_MAIN - 1,
        0,
        message_thread,
        NULL,
        "message_thread"
    );

    printf("Message box thread started with PID %d\n", thread_pid);

    return 0;
}