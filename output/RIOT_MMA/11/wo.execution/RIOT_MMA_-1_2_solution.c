#include <thread.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>

#define SLEEP_DURATION_SEC 5

static mma8x5x_t dev;
static mma8x5x_data_t data;

// Parameters specific for the accelerometer initialization
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_mma8x5x(void *arg) {
    (void)arg;
    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        
        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_DURATION_SEC);
    }
    return NULL;
}

int main(void) {
    puts("Initializing the MMA8x5x accelerometer...");
    
    // Initialize I2C and MMA8x5x accel device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }
    
    // Start the read thread
    kernel_pid_t read_thread = thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "read_mma8x5x");
    if (read_thread < 0) {
        puts("Failed to create thread");
        return 1;
    }

    return 0;
}