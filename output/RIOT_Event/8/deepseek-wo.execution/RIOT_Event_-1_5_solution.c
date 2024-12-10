
#include <stdlib.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <MQTTClient.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the timer
    ztimer_t timer;
    ztimer_set_periodic(ZTIMER_USEC, &timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        // Do nothing, just keep the main thread alive
    }

    return 0;
}
