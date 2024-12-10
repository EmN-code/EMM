#include <xtimer.h>
#include <periph/i2c.h>
#include <stdio.h>

#define DELAY 5U * US_PER_SEC

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void mma8x5x_read(mma8x5x_t *dev, mma8x5x_data_t *data) {
    // TODO: Implement actual I2C read sequence from the sensor
    // For the sake of example, we are just setting static values
    data->x = 100; // Replace with actual read value
    data->y = 200; // Replace with actual read value
    data->z = -50; // Replace with actual read value
}

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res == MMA8X5X_OK) {
        puts("MMA8x5x initialization successful");
    } else {
        printf("MMA8x5x initialization failed: %d\n", res);
        return -1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Read values - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(DELAY);
    }

    return 0;
}