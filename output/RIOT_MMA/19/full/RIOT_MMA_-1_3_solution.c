#include "thread_config.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include <unistd.h>

static mma8x5x_t dev;
mma8x5x_data_t data;

void *read_sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        }
        sleep(5);
    }
    return NULL;
}

int main(void) {
    // Assume CONFIG_MMA8X5X_I2C_ADDRESS and correct rate and range are defined elsewhere
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // assuming this is defined correctly elsehwre
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x!\n");
        return 1;
    }
    
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor_thread, NULL, "sensor_reader");

    return 0;
}