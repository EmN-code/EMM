#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>

// Definitions
#define TIMER_INTERVAL (1U * ZTIMER_MSEC) // 1-second interval
#define MAIN_QUEUE_SIZE (8)

static msg_t msg_queue[MAIN_QUEUE_SIZE];
static msg_t msg;
static mbox_t mbox;

// Timer handler function
void timer_handler(void *arg) {
    (void)arg;
    printf("Timer expired, sending event to message box.\n");
    msg.content.value = rand() % 100;  // Simulate a message with a random payload
    msg_try_send(&mbox, &msg);
}

int main(void) {
    printf("RIOT application to send events via a message box.\n");
    
    // Initialize msg and mbox
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MAIN_QUEUE_SIZE);

    // Set up and start a timer
    ztimer_t timer;
    timer.callback = timer_handler;
    timer.arg = NULL;
    ztimer_start(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    // Main loop
    while (1) {
        // Here we will just print that we're waiting for events, 
        // in a real application, you may process events
        printf("Waiting for events...\n");
        msg_receive(&msg);
        printf("Received event with value: %d\n", msg.content.value);
    }
    return 0;
}