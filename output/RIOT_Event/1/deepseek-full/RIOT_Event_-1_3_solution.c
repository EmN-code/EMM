
#include "xtimer.h"
#include "msg.h"
#include "periph/gpio.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static msg_t msg_queue[8];
static xtimer_t periodic_timer;

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize and start the periodic timer
    xtimer_init(&periodic_timer);
    xtimer_set_periodic(&periodic_timer, INTERVAL_MS, (xtimer_callback_t)periodic_callback, NULL);

    // Main loop to process messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
