#include "mma8x5x.h"
#include "xtimer.h"
#include "periph/i2c.h"

#define I2C_BUS         I2C_DEV(0)  // Adjust based on your setup
#define MMA8X5X_ADDR    0x1D        // Default I2C address for MMA8x5x
#define READ_INTERVAL   (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void)
{
    // Initialize I2C
    i2c_init(I2C_BUS);

    // Initialize the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_BUS,
        .addr = MMA8X5X_ADDR,
        .rate = 0,  // Set appropriate rate
        .range = 0, // Set appropriate range
    };
    dev.params = params;

    // Main loop
    while (1) {
        read_sensor();
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}