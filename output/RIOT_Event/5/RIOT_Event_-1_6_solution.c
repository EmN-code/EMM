
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time_units.h>
#include <thread.h>
#include <periph/gpio.h>
#include <arpa/inet.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <timex.h>
#include <mbox.h>
#include <periph/i2c.h>
#include <msg.h>
#include <dht.h>
#include <dht_params.h>
#include <kernel_defines.h>
#include <netinet/in.h>

// Define a simple callback function
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_MSEC, &event_queue_get(), periodic_callback, NULL);

    // Start the periodic callback with an interval of 1 second
    event_periodic_callback_start(&event, 1000);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
