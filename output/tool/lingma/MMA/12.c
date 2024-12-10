#include "riot.h"
#include "board.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "fmt.h"

#define I2C_DEV (I2C_DEV(0))  // Use I2C device 0
#define I2C_SPEED (I2C_SPEED_FAST)  // Set I2C speed to fast (400 kHz)

void init_i2c(void) {
    i2c_init(I2C_DEV, I2C_SPEED);
}

mma8x5x_t dev;

void init_mma8x5x(void) {
    if (mma8x5x_init(&dev, I2C_DEV) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }
    puts("MMA8x5x initialized successfully");
}

void read_mma8x5x(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&dev, &data) != MMA8X5X_OK) {
        puts("Failed to read data from MMA8x5x");
        return;
    }

    char buffer[64];
    fmt(buffer, "X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    puts(buffer);
}

int main(void) {
    puts("Starting MMA8x5x example on ESP32");

    init_i2c();
    init_mma8x5x();

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}