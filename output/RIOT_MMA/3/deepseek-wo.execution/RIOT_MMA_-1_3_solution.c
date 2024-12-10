
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 0x06
#define MMA8X5X_RANGE_2G 0x00

typedef struct {
    int i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
} mma8x5x_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    // Simulated initialization function
    return 0;
}

void mma8x5x_read(mma8x5x_t *dev, mma8x5x_data_t *data) {
    // Simulated read function
    data->x = 100;
    data->y = 200;
    data->z = 300;
}

int main() {
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = open("/dev/i2c-1", O_RDWR),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (params.i2c < 0) {
        perror("Failed to open I2C device");
        return 1;
    }

    if (ioctl(params.i2c, I2C_SLAVE, params.addr) < 0) {
        perror("Failed to set I2C address");
        close(params.i2c);
        return 1;
    }

    if (mma8x5x_init(&dev, &params) != 0) {
        fprintf(stderr, "Failed to initialize MMA8x5x accelerometer\n");
        close(params.i2c);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        sleep(5);
    }

    close(params.i2c);
    return 0;
}
