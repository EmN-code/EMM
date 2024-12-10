#include <unistd.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <periph/i2c.h>
#include <stdio.h>

#define I2C_DEV      I2C_DEV(0)             // Ensure this matches your setup
#define ADDR         MMA8X5X_I2C_ADDRESS
#define RATE         MMA8X5X_RATE_50HZ
#define RANGE        MMA8X5X_RANGE_2G
#define SLEEP_TIME   5 * US_PER_SEC // 5 seconds in microsecond precision

static char stack[THREAD_STACKSIZE_MAIN];
static mma8x5x_t dev;

void *sensor_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t acc_data;
    while (1) {
        if (mma8x5x_read(&dev, &acc_data) != 0) {
            printf("Failed to read accelerometer data\n");
        } else {
            printf("Acceleration: x=%d, y=%d, z=%d\n", acc_data.x, acc_data.y, acc_data.z);
        }
        xtimer_usleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    puts("Initializing MMA8x5x sensor...");

    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = ADDR;
    params.rate = RATE;
    params.range = RANGE;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    puts("MMA8x5x initialized.");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sensor_thread, NULL, "sensor_thread");

    return 0;
}
