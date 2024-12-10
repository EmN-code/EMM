#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <xtimer.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <stdlib.h>
#include <stdbool.h>

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Define the correct I2C address for your setup

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *mma8x5x_thread(void *arg) {
    (void)arg;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize mma8x5x\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    puts("MMA8x5x acceleration sensor application\n");

    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mma8x5x_thread, NULL, "mma8x5x_thread");

    return 0;
}