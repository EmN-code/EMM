
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the message queue for the current thread
    msg_init_queue(msg_array, ARRAY_SIZE(msg_array));

    // Set up the periodic timer
    timer.target = xtimer_now_usec();
    timer.long_target = timer.target + PERIODIC_INTERVAL;
    timer.callback = (xtimer_callback_t)periodic_callback;
    timer.arg = NULL;

    // Start the periodic timer
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
