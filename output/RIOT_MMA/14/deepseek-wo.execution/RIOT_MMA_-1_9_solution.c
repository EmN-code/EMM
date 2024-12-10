
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <i2c/i2c.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 0x06
#define MMA8X5X_RANGE_2G 0x00

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    mma8x5x_params_t params;
    i2c_t i2c;
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    dev->params = *params;
    dev->i2c = params->i2c;
    return 0;
}

void mma8x5x_read(const mma8x5x_t *dev, mma8x5x_data_t *data) {
    uint8_t buffer[6];
    i2c_acquire(dev->i2c);
    i2c_read_regs(dev->i2c, dev->params.addr, 0x01, buffer, 6, 0);
    i2c_release(dev->i2c);

    data->x = (buffer[0] << 8) | buffer[1];
    data->y = (buffer[2] << 8) | buffer[3];
    data->z = (buffer[4] << 8) | buffer[5];
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        sleep(5);
    }

    return 0;
}
