#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <kernel_defines.h>
#include <mma8x5x.h>
#include <event/thread.h>
#include <dht_params.h>
#include <MQTTClient.h>

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor_thread(void *arg) {
    while (true) {
        if (mma8x5x_is_ready(&dev)) {
            if (mma8x5x_read(&dev, &data) == 0) {
                printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);  
            }
            else {
                printf("Failed to read data\n");
            }
        }
        else {
            printf("Device not ready\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Error initializing device\n");
        return 1;
    }

    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor_thread, NULL, "sensor_thread");

    return 0;
}
