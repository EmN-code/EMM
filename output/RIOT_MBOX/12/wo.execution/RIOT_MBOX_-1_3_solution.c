#include <ztimer.h>
#include <thread_config.h>
#include <msg.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_SEC) // Define the timer interval in seconds
#define MAIN_QUEUE_SIZE (8)

static msg_t main_msg_queue[MAIN_QUEUE_SIZE];

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer event occurred!\n");
    // Logic for handling the timer event, such as sending a message
}

int main(void) {
    msg_init_queue(main_msg_queue, MAIN_QUEUE_SIZE);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL; // Optional: set if callback requires an argument

    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg); // Block waiting for a message
        // Additional processing based on message received
    }

    return 0;
}