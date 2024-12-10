#include <ztimer.h>  // Include the ztimer header for timer functionalities
#include <stdio.h>    // Include the standard input-output header
#include <msg.h>      // Include the message API header

#define TIMER_INTERVAL (1000000U) // Timer interval set to 1 second (in microseconds)
#define QUEUE_SIZE     (8)        // Define the size of the message queue

static msg_t queue[QUEUE_SIZE];  // Create the message queue
static ztimer_periodic_t timer;  // Define a periodic timer object

static void _timeout_cb(void *arg) {
    (void)arg;  // Explicitly ignore the parameter if not used
    puts("Timer callback fired!");
    msg_t msg;
    msg_send(&msg, 0);  // Send a message to the message queue
}

int main(void) {
    puts("RIOT ESP32 Message Box Example");

    // Initialize the message queue
    msg_init_queue(queue, QUEUE_SIZE);

    // Configure the periodic timer and associate it with a callback function
    ztimer_periodic_init(ZTIMER_USEC, &timer, _timeout_cb, NULL, TIMER_INTERVAL);

    // Start the periodic timer
    ztimer_periodic_start(&timer);

    // Main loop to process messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);  // Block until a message is received
        puts("Message received!");
    }

    return 0;
}