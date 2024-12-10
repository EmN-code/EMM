#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

#define MMA8X5X_ADDRESS 0x1D
#define I2C_DEV        I2C_DEV(0)
#define I2C_SPEED      I2C_SPEED_FAST

static mma8x5x_t mma8x5x;
static mma8x5x_params_t mma8x5x_params = {
    .i2c_dev = I2C_DEV,
    .i2c_addr = MMA8X5X_ADDRESS,
};

int main(void) {
    int res;
    printf("Initializing MMA8x5x sensor...\n");

    while (1) {
        res = mma8x5x_init(&mma8x5x, &mma8x5x_params);
        if (res != 0) {
            printf("Failed to initialize MMA8x5x sensor. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("MMA8x5x sensor initialization successful.\n");
            break;
        }
    }

    while (1) {
        mma8x5x_measurement_t data;
        res = mma8x5x_read(&mma8x5x, &data);
        if (res != 0) {
            printf("Failed to read from MMA8x5x sensor. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Read successful. X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(5);
    }

    return 0;
}