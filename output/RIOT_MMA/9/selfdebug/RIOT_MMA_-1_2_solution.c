// This code reads acceleration data from an MMA8x5x accelerometer connected to an ESP32 every 5 seconds.
#include <thread_arch.h>
#include <periph_conf.h>
#include <stdio.h>

#include "mma8x5x.h"

#define READ_INTERVAL (5U)

static mma8x5x_t dev;
static const mma8x5x_params_t sensor_params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void)arg;
    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        thread_sleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize the sensor
    if (mma8x5x_init(&dev, &sensor_params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }
    
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_sensor, NULL, "accel_thread");
    if(pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}