#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "event.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define INTERVAL_USEC    (1000000U) /* 1 second interval */

static msg_t msg;
static kernel_pid_t main_pid;
static event_queue_t event_queue;
static ztimer_t timer;

void callback(void *arg) {
    (void)arg;
    msg_send(&msg, main_pid); // Send message to main thread
    ztimer_set(&timer, INTERVAL_USEC); // Reset timer for next event
}

int main(void) {
    main_pid = thread_get_active(); // Obtain PID of the main thread
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE); // Initialize message queue
    event_queue_init(&event_queue, MAIN_QUEUE_SIZE);

    ztimer_init(ZTIMER_MSEC); // Initialize ztimer
    ztimer_set_callback(&timer, callback, NULL); // Set callback for timer
    ztimer_set(&timer, INTERVAL_USEC); // Start timer
    
    while (1) {
        // Process events periodically
        if (msg_receive(&msg) == 1) {
            puts("Event received!");
            // Handle events here
        }
    }
    return 0;
}
