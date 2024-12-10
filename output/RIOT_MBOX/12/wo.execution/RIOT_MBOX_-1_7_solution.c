#include <ztimer.h>  // Timer defines
#include <thread.h>  // Thread handling
#include <stdio.h>   // Standard input/output
#include <msg.h>     // Message passing API

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define MSG_QUEUE_SIZE           (8)
#define TIMER_INTERVAL           (1U * ZTIMER_MSEC_PER_SEC)  // 1 second

static msg_t msg_queue[MSG_QUEUE_SIZE];

// Timer callback function
void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0; // Define a message type
    printf("Sending message from timer callback.\n");
    msg_send(&msg, thread_getpid());
}

int main(void) {
    ztimer_t timer;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    printf("Message queue initialized.\n");

    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, thread_getpid());
    timer.callback = timer_callback;
    timer.arg = NULL;
    printf("Timer set for %u milliseconds intervals.\n", TIMER_INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }

    return 0;
}