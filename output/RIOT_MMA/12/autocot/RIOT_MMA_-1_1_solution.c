#include <thread.h>
#include <periph_conf.h>
#include <inttypes.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL    (5U)

// MMA8x5x device parameters
mma8x5x_params_t mma_params = {
    // Configure your params here, e.g.,
    // .i2c = I2C_DEV(
};
mma8x5x_t mma_dev;

int read_mma8x5x(void)
{
    if (mma8x5x_is_ready(&mma_dev)) {
        int16_t x, y, z;
        mma8x5x_read(&mma_dev, &x, &y, &z);
        printf("MMA8x5x readings: x=%d, y=%d, z=%d\n", x, y, z);
    } else {
        printf("MMA8x5x not ready\n");
    }
    return 0;
}

void *mma_thread(void *arg)
{
    (void)arg;

    // Initialize the sensor
    if (mma8x5x_init(&mma_dev, &mma_params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    // Loop to read the MMA8x5x sensor every 5 seconds
    while (1) {
        read_mma8x5x();
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Create a thread for the MMA8x5x sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma_thread, NULL, "mma_thread");

    return 0;
}
