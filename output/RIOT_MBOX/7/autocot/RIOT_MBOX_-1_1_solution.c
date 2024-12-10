#include <ztimer.h>  // For ztimer functionality
#include <stdio.h>   // For standard input/output functions
#include <msg.h>     // For message box and message handling

#define MAIN_QUEUE_SIZE     (8)      // Size of the message queue
#define TIMER_INTERVAL      (1000000U)  // Timer interval in microseconds (1 second)

static msg_t main_queue[MAIN_QUEUE_SIZE];

void timer_callback(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;  // Just a random message type number
    msg.content.value = (long) ztimer_now(ZTIMER_USEC);
    printf("Sending message with timestamp: %ld\n", msg.content.value);
    msg_send(&msg, thread_getpid());  // Send message to the current thread's message box
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(main_queue, MAIN_QUEUE_SIZE);

    // Setting up a periodic timer
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_USEC, TIMER_INTERVAL);
    timer.callback = timer_callback;
    printf("Timer set to fire every %u microseconds.\n", TIMER_INTERVAL);

    // Main message loop
    msg_t msg;
    while (1) {
        if (msg_receive(&msg)) {
            printf("Received message with timestamp: %ld\n", msg.content.value);
        }
    }

    return 0;
}