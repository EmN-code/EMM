
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <stdbool.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <dht_params.h>
#include <MQTTClient.h>

// Define a simple callback function for the periodic event
void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the RTC for timekeeping
    rtc_init();

    // Set up a periodic event using the RTC
    uint32_t interval = 1000000; // 1 second in microseconds
    rtc_timer_t timer;
    rtc_timer_init(&timer);

    // Start the periodic event
    while (1) {
        rtc_timer_set_periodic(&timer, interval);
        periodic_callback(NULL);
        rtc_timer_wait(&timer);
    }

    return 0;
}
