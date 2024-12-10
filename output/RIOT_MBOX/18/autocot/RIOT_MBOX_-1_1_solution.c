// The code sets up a mailbox and sends events at regular intervals using RIOT OS.
// We utilize the event timer and mailbox functionality provided by RIOT.

#include <ztimer.h>  // Provides ztimer for timer functions
#include <xtimer.h>  // Provides xtimer for precise delays
#include <msg.h>     // Provides message struct and functions

#define INTERVAL (1U * US_PER_SEC)  // Time interval for sending messages (1 second)
#define STACK_SIZE (THREAD_STACKSIZE_MAIN)
#define QUEUE_SIZE (8)

// Thread stack definition
char stack[STACK_SIZE];

// Mailbox message queue
msg_t queue[QUEUE_SIZE];

// Timer and semaphore for control
static ztimer_t timer;

// This function is called when the timer expires
static void _timer_callback(void *arg)
{
    (void)arg;  // Unused parameter
    msg_t msg;
    msg.content.value = 42;  // Set some arbitrary content
    printf("Timer expired, sending message...\n");
    msg_send(&msg, thread_getpid());
}

// Main function
int main(void)
{
    // Initialize the mailbox
    msg_init_queue(queue, QUEUE_SIZE);

    // Initialize the timer
    ztimer_set_msg(ZTIMER_USEC, &timer, INTERVAL, thread_getpid(), NULL);
    timer.callback.arg = NULL;
    timer.callback.func = _timer_callback;

    while (1) {
        // Receive messages
        msg_t msg;
        msg_receive(&msg);

        // Handle messages
        printf("Received message with value: %d\n", (int)msg.content.value);
    }

    return 0;
}