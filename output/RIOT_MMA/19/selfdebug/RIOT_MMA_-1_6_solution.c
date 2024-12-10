#include "periph_conf.h"
#include "periph/i2c.h"
#include "ztimer.h"
#include "stdio.h"

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 50
#define MMA8X5X_RANGE_2G 2
#define ZTIMER_MSEC 1000

typedef struct {
    i2c_t i2c;                  /**< I2C bus the device is connected to */
    uint8_t addr;               /**< I2C bus address of the device */
    uint8_t rate;               /**< sampling rate to use */
    uint8_t range;              /**< scale range to use */
    uint8_t offset[3];          /**< data offset in X, Y, and Z direction */
} mma8x5x_params_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mma8x5x_data_t;

typedef struct {
    mma8x5x_params_t params;    /**< device configuration parameters */
} mma8x5x_t;

int mma8x5x_init(mma8x5x_t *dev, const mma8x5x_params_t *params) {
    // Assume successful initialization for mock-up purposes
    printf("Initialized with I2C address: 0x%x\n", params->addr);
    return 0; // MMA8X5X_OK
}

void mma8x5x_read(mma8x5x_data_t *data) {
    // Mock read function, replace with actual hardware reading logic
    data->x = 100;
    data->y = 100;
    data->z = 100;
}

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000); // sleep for 5000 milliseconds
    }

    return 0;
}