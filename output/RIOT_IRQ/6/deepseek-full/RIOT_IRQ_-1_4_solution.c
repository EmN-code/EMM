
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <ztimer.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>
#include <stddef.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>

#define STACKSIZE 512
#define PRIORITY 5
#define MAX_RETRIES 5

volatile int atomic_var = 0;

void interrupt_handler(void *arg) {
    (void)arg;
    atomic_var = 1;
    printf("Interrupt handled, atomic_var set to 1\n");
}

int main(void) {
    msg_t msg_queue[8];
    mbox_t mbox;
    evtimer_msg_t evtimer;

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_mbox(&evtimer, &mbox);

    // Simulate thread creation with retries
    kernel_pid_t pid = -1;
    for (int attempts = 0; attempts < MAX_RETRIES; attempts++) {
        // Simulate thread creation failure
        if (attempts < MAX_RETRIES - 1) {
            printf("Simulating thread creation failure attempt %d\n", attempts + 1);
        } else {
            printf("Simulating successful thread creation on attempt %d\n", attempts + 1);
            pid = 1; // Simulate successful thread creation
        }

        if (pid > 0) {
            break;
        } else if (attempts == MAX_RETRIES - 1) {
            printf("Failed to create thread after %d attempts\n", attempts + 1);
            return 1;
        }
    }

    // Main loop to handle interrupts
    while (1) {
        if (atomic_var == 1) {
            printf("Handling interrupt...\n");
            interrupt_handler(NULL);
            atomic_var = 0;
        } else {
            // Simulate sleeping
            xtimer_usleep(100000); // Sleep for 100ms
        }
    }

    return 0;
}
