// Required Imports
diff --git a/main.c b/main.c
#include <ztimer.h>
#include <xtimer.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include <unistd.h>
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include <string.h>
#include "MQTTClient.h"
#include "timeline/timeout.h"

// Global Variables
static mma8x5x_t dev;
mma8x5x_data_t data;

// Main function
int main(void) {
    // Initialize MMA8X5X accelerometer
    mma8x5x_params_t params = {
        .i2c = CONFIG_I2C_DEV,  // Using CONFIG_I2C_DEV instead of I2C_DEV macro
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int rc;
    rc = mma8x5x_init(&dev, &params);
    if (rc != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    // Network connection handling
    Network network;
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }

    // Periodically read accelerometer data
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%dmg, y=%dmg, z=%dmg\n", data.x_axis, data.y_axis, data.z_axis);
        ztimer_sleep(ZTIMER_SEC, 5);  // Sleep for 5 seconds
    }

    return 0;
}