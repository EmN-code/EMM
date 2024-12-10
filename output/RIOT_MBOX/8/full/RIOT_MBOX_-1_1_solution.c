#include <ztimer.h>
#include <string.h>
#include <msg.h>

#define INTERVAL (1000000U) // Interval in microseconds for periodic events

static msg_t msg_queue[8];
static ztimer_t timer;

/* Timer callback function to send an event message */
static void timer_callback(void *arg)
{
    (void)arg;
    msg_t msg;
    msg.content.value = 42;   // Example message content
    msg_send(&msg, msg_queue);    // Send message
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, 8);
    
    // Initialize timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);

    // Main event loop
    while (1) {
        msg_t msg;
        msg_receive(&msg);              // Wait for and receive message
        printf("Received message with content: %d\n", msg.content.value);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);  // Wait for the specified interval
    }

    return 0;
}