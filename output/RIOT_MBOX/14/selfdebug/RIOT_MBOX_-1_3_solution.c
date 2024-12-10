#include <ztimer.h>   // For timer functions and types
#include <thread.h>   // For thread functions and types
#include <stdio.h>    // For standard I/O functions
#include <msg.h>      // For message box functions

#define INTERVAL 1000000  // 1 second in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];  // Message queue of size 8

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    ztimer_clock_t last_wakeup = ztimer_now(ZTIMER_SEC);

    while (1) {
        ztimer_periodic_wakeup(ZTIMER_SEC, &last_wakeup, INTERVAL / US_PER_SEC);
        msg.content.value = ztimer_now(ZTIMER_SEC);
        msg_send(&msg, thread_getpid());
        printf("Message sent with timestamp: %lu\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, thread_handler, NULL,
                  "sender_thread");

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message with timestamp: %lu\n", msg.content.value);
    }
    return 0;
}