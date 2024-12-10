
#include "esp_system.h"
#include "msg.h"
#include "xtimer.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U) // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Main loop to receive messages
    while (1) {
        msg_receive(&msg);
    }

    return 0;
}
