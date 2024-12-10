#include <stdio.h>
#include <periph/i2c.h>
#include <periph_cpu.h>

#define US_PER_SEC 1000000

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Sensor initialization failed\n");
        return;
    }
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_usleep(5000000);  // Sleep for 5 seconds
    }
}

int main(void)
{
    puts("Starting MMA8x5x sensor reading...");
    read_sensor();
    return 0;
}