#include <thread.h>
#include <timex.h>
#include <xtimer.h>
#include <sensor/mma8x5x.h>
#include <periph/i2c.h>

#define I2C_DEV        I2C_DEV(0)  // Use I2C interface 0
#define MMA8X5X_ADDR   (0x1D)      // Default I2C address for MMA8x5x
#define READ_INTERVAL  (5U)        // Read interval in seconds

static mma8x5x_t mma8x5x_dev;
static mma8x5x_params_t mma8x5x_params = {
    .i2c = I2C_DEV,
    .address = MMA8X5X_ADDR
};

void *sensor_read_thread(void *arg) {
    (void)arg;
    mma8x5x_data_t data;

    while (1) {
        if (mma8x5x_read(&mma8x5x_dev, &data) == 0) {
            printf("Acceleration: X=%d Y=%d Z=%d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting MMA8x5x sensor application\n");

    if (mma8x5x_init(&mma8x5x_dev, &mma8x5x_params) != 0) {
        puts("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sensor_read_thread, NULL, "sensor_read");

    return 0;
}