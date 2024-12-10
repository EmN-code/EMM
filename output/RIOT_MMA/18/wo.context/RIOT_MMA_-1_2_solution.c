#include "thread.h"
#include "xtimer.h"
#include "periph/i2c.h"

// MMA8x5x includes
#include "mma8x5x.h"

#define I2C_DEV          I2C_DEV(0)
#define MMA8x5x_ADDR     (0x1D)  // Assuming 0x1D as the I2C address for MMA8x5x

// Thread stack size
#define STACK_SIZE       THREAD_STACKSIZE_DEFAULT

// Global variables for mma8x5x device and parameters
static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_sensor(void *arg) {
    (void)arg;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8x5x_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8x5x sensor\n");
        return NULL;
    }

    // Reading loop
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Failed to read data from MMA8x5x sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a thread to read the sensor
    char stack[STACK_SIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_sensor, NULL, "read_sensor");
    return 0;
}