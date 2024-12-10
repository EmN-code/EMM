#include "thread_config.h"
#include "periph/cpu.h"
#include "xtimer.h"
#include "msg.h"
#include "mma8x5x.h"
#include "i2c.h"

#define I2C_INTERFACE I2C_DEV(0) // Use the appropriate I2C interface
#define MMA8X5X_I2C_ADDRESS 0x1D // Default I2C address for MMA8x5x

// mma8x5x device descriptor
static mma8x5x_t dev;

void read_accelerometer(void) {
    mma8x5x_data_t data;
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("X: %d\tY: %d\tZ: %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Error reading from accelerometer\n");
    }
}

void *thread_func(void *arg) {
    (void)arg;
    while (1) {
        read_accelerometer();
        xtimer_sleep(5); // Sleeps for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Specify parameters
    mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    // Create a separate thread for reading
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1, 0, thread_func, NULL, "mma8x5x_reader");

    return 0;
}