#include "mqttclient.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Example I2C address, set it according to the device datasheet
#define SLEEP_INTERVAL (5U * US_PER_SEC) // Sleep interval of 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params;

    params.i2c = I2C_DEV(0);  // Assuming using the first I2C device, update if needed
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialization
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Read data from sensor
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }

        // Sleep for defined interval
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}