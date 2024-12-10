// Including necessary headers
#include "stdint.h" // For standard integer types
#include "kernel.h" // For kernel thread functions
#include "periph/i2c.h" 
#include "mma8x5x.h" // For the MMA8x5x sensor functions

#define I2C_DEV         (I2C_DEV(0))
#define MMA8X5X_ADDR    (0x1D)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE (512U)
#define READ_INTERVAL    (5U * US_PER_SEC) // 5 seconds in microseconds

// Device descriptor
static mma8x5x_t dev;
static mma8x5x_data_t data;

// Thread stack
static char thread_stack[THREAD_STACKSIZE];

// Parameters for MMA8x5x
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to read accelerometer data
void *read_mma8x5x(void *arg) {
    (void)arg;
    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        // Sleep for the read interval
        xtimer_usleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the MMA8x5x
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Create a thread to read data
    kernel_pid_t read_pid = thread_create(
        thread_stack, THREAD_STACKSIZE,
        THREAD_PRIORITY, THREAD_CREATE_STACKTEST,
        read_mma8x5x, NULL, "read_mma8x5x");

    if (read_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}
