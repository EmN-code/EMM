#include <thread.h>
#include <stdio.h>
#include <periph_conf.h>
#include <mma8x5x.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define SLEEP_TIME               (5U)           // 5 seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),           // using I2C_DEV(0) for example
    .addr = MMA8X5X_I2C_ADDRESS  // default I2C address for MMA8x5x
};

void *read_sensor(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            printf("MMA8x5x - x: %d, y: %d, z: %d\n", x, y, z);
        } else {
            printf("Sensor not ready\n");
        }
        xtimer_sleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("MMA8x5x: initialization failed\n");
        return 1;
    }
    printf("MMA8x5x: initialized\n");

    thread_create((char *)thread_stack, THREAD_STACKSIZE_MAIN,
                   THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                   read_sensor, NULL, "read_sensor");

    return 0;
}