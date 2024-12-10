#include "thread_config.h"
#include "thread_arch.h"
#include "thread_flags.h"
#include "thread.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include "inttypes.h"
#include "host/ble_ibeacon.h"
#include "host/ble_hs_adv.h"
#include "hashes/sha2xx_common.h"

#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "mma8x5x.h"

#define SLEEP_PERIOD 5 // seconds

// Declare the MMA8x5x device variable
mma8x5x_t dev;

void read_sensor(void) {
    int16_t x, y, z;

    // Read acceleration data from the MMA8x5x
    if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
        printf("Error reading from MMA8x5x\n");
        return;
    }

    // Print the acceleration data
    printf("Acceleration: X=%d, Y=%d, Z=%d\n", x, y, z);
}

int main(void) {
    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        .params = {0}   // Initialize all parameters to 0 or set as needed
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    printf("MMA8x5x initialized\n");

    // Periodically read the sensor data
    while (1) {
        read_sensor();
        xtimer_sleep(SLEEP_PERIOD);
    }

    return 0;
}