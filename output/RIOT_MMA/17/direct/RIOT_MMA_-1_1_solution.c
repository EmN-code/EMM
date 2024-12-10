#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/i2c.h>

#define MMA8x5x_ADDR 0x1D  // Example I2C address for MMA8x5x
#define I2C_DEV I2C_DEV(0) // I2C device interface
#define SLEEP_TIME (5U * 1000000U)  // 5 seconds in microseconds

// Function to read data from MMA8x5x
static int read_mma8x5x(void) {
    uint8_t data[6];
    if (i2c_acquire(I2C_DEV) != 0) {
        puts("Failed to acquire I2C");
        return -1;
    }
    if (i2c_read_regs(I2C_DEV, MMA8x5x_ADDR, 0x00, data, sizeof(data), 0) != 0) {
        puts("Failed to read data from MMA8x5x");
        i2c_release(I2C_DEV);
        return -1;
    }
    i2c_release(I2C_DEV);

    // Process raw data (assuming 2 bytes per axis output)
    int16_t x = (data[0] << 8) | data[1];
    int16_t y = (data[2] << 8) | data[3];
    int16_t z = (data[4] << 8) | data[5];

    printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    return 0;
}

int main(void)
{
    i2c_init(I2C_DEV);

    while (1) {
        read_mma8x5x();
        xtimer_usleep(SLEEP_TIME);
    }

    return 0;
}