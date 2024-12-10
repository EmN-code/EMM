#include "xtimer.h"
#include "thread.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t sensor_data;

#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    // Initialize the parameters for the MMA8x5x accelerometer
    const mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Handle I2C initialization and test connection
    while (1) {
        int res = mma8x5x_init(&dev, &params);
        if (res != MMA8X5X_OK) {
            printf("Failed to initialize MMA8x5x device: %d\n", res);
            xtimer_sleep(1);
            continue;
        } else {
            printf("MMA8x5x initialized successfully.\n");
            break;
        }
    }

    // Start the loop that reads sensor data every 5 seconds
    while (1) {
        int res = mma8x5x_read(&dev, &sensor_data);
        if (res == MMA8X5X_OK) {
            printf("Acceleration data - X: %d, Y: %d, Z: %d\n",
                   sensor_data.x_axis, sensor_data.y_axis, sensor_data.z_axis);
        } else {
            printf("Failed to read sensor data: %d\n", res);
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}