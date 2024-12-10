#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "time_units.h"
#include "unistd.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "mqtt_paho.h"
#include "MQTTClient.h"
#include "periph/gpio.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "kernel_defines.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "thread.h"
#include "periph/rtc.h"
#include "mma8x5x.h"
#include "stdio.h"

#define MMA8X5X_I2C_ADDRESS   0x1D // replace this with the actual address
#define THREAD_PRIO_MAIN      1

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_mma8x5x(void *arg) {
    (void)arg;

    // Initialize accelerometer params
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return NULL;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("Acceleration - X: %dmg, Y: %dmg, Z: %dmg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        ::sleep(5);
    }

    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];

    // Create a thread for reading accelerometer
    thread_create(stack, sizeof(stack), THREAD_PRIO_MAIN, 0, read_mma8x5x, NULL, "mma8x5x_reader");

    return 0;
}