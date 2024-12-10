
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <kernel_defines.h>
#include <stdatomic.h>

#define SLEEP_DURATION 5000000  // 5 seconds in microseconds

static void sleep_for_5_seconds(void) {
    ztimer_sleep(ZTIMER_USEC, SLEEP_DURATION);
}

int main(void) {
    while (1) {
        // Simulate reading MMA8x5x accelerometer data
        printf("Reading MMA8x5x accelerometer data...\n");

        // Simulate processing the data
        printf("Processing accelerometer data...\n");

        // Sleep for 5 seconds
        sleep_for_5_seconds();
    }

    return 0;
}
