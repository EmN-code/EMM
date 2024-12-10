#include <xtimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <time_units.h>
#include <thread.h>
#include <timex.h>
#include <unistd.h>
#include <ztimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <periph_conf.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <kernel_defines.h>

#define CONFIG_MMA8X5X_I2C_ADDRESS (0x1D) // Example address, replace with appropriate value

static kernel_pid_t _main_thread_pid;
static char _stack[THREAD_STACKSIZE_MAIN];
static mma8x5x_t _mma8x5x_dev;
static mma8x5x_data_t _mma8x5x_data;

void *_mma8x5x_thread(void *arg) {
    (void)arg;

    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&_mma8x5x_dev, &_mma8x5x_data);
        
        // Calculate acceleration in mg
        int16_t x_a = _mma8x5x_data.x;
        int16_t y_a = _mma8x5x_data.y;
        int16_t z_a = _mma8x5x_data.z;
        
        printf("Acceleration in X: %d mg, Y: %d mg, Z: %d mg\n", x_a, y_a, z_a);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void){
    // Initialize the I2C device and sensor configuration
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&_mma8x5x_dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Create the thread to handle reading
    _main_thread_pid = thread_create(_stack, sizeof(_stack), THREAD_PRIORITY_MAIN - 1,
                                      THREAD_CREATE_STACKTEST, _mma8x5x_thread, NULL, "mma8x5x_thread");

    if (_main_thread_pid <= KERNEL_THREAD_IDLE_PID) {
        puts("Failed to create thread\n");
        return 1;
    }

    return 0;
}
