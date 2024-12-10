#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "board.h"

static i2c_t i2c_dev;
static mma8x5x_t dev;

void init_sensor(void) {
    // Initialize I2C
    i2c_init(&i2c_dev, I2C_DEV(0));  // Assuming I2C device 0
    i2c_acquire(&i2c_dev);

    // Initialize MMA8x5x
    mma8x5x_params_t params = {
        .i2c = &i2c_dev,
        .addr = MMA8X5X_I2C_ADDRESS
    };

    mma8x5x_init(&dev, &params);
}

void read_sensor_data(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the sensor
    init_sensor();

    while (1) {
        // Read sensor data
        read_sensor_data();

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}

