
#include "xtimer.h"
#include "thread.h"
#include "event/thread.h"
#include "esp_system.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    event_t event;

    // Initialize the event
    event_init(&event, (event_callback_t)periodic_callback, NULL);

    // Set the timer to trigger the event every second
    xtimer_set(&timer, PERIODIC_INTERVAL, (xtimer_callback_t)event_post, &event);

    // Main loop to keep the thread running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
