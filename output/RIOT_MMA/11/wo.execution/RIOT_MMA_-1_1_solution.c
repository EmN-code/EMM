#include <xtimer.h>
#include <thread.h>
#include <saul_reg.h>
#include <periph_conf.h>
#include <mma8x5x.h>

#define INTERVAL (5U)

static mma8x5x_t dev;

void *sensor_reading_thread(void *arg) {
    (void)arg;
    mma8x5x_acc_t data;

    while (1) {
        if (mma8x5x_read(&dev, &data) == 0) {
            printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading accelerometer data\n");
        }
        xtimer_sleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c         = I2C_DEV(0),
        .addr        = MMA8X5X_ADDR,
        .scale       = MMA8X5X_SCALE_AUTO,
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization of MMA8x5x failed\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sensor_reading_thread, NULL, "Sensor Reader");

    return 0;
}