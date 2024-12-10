#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "msg.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 second interval

static char thread_stack[THREAD_STACKSIZE_MAIN];

// Device descriptor
static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *sensor_thread(void *arg) {
    (void)arg;
    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer\n");
        return NULL;
    }

    while (1) {
        // Read the sensor data
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Create a thread to handle sensor readings
    kernel_pid_t sensor_pid = thread_create(thread_stack, sizeof(thread_stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            sensor_thread, NULL, "sensor thread");
    if (sensor_pid <= KERNEL_PID_UNDEF) {
        printf("Error: unable to create thread\n");
    }

    return 0;
}