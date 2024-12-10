#include "ztimer.h"
#include "thread.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "fmt.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        /* Read the sensor data */
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            /* Print the sensor data */
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading the sensor\n");
        }
        /* Sleep for 5 seconds */
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    /* Initialize the sensor */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the sensor\n");
        return 1;
    }

    /* Create the sensor thread */
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sensor_thread, NULL, "sensor_thread");

    return 0;
}