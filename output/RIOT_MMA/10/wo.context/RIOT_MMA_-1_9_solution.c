#include "mma8x5x/mma8x5x.h"
#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "periph/i2c.h"

// Define the I2C device
#define I2C_DEV         (I2C_DEV(0))

// Define the MMA8x5x parameters
#define MMA8X5X_ADDR    (0x1D)  // Assuming this is the default I2C address for MMA8653
#define READ_INTERVAL   (5U * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_params_t params;
static mma8x5x_data_t data;

void read_mma8x5x(void) {
    // Initialize the MMA8x5x parameters
    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_ADDR;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize the MMA8x5x device\n");
        return;
    }

    // Read data from the MMA8x5x sensor
    mma8x5x_read(&dev, &data);
    printf("X-axis: %d, Y-axis: %d, Z-axis: %d\n", data.x, data.y, data.z);
}

int main(void) {
    // Run the read function every 5 seconds
    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}