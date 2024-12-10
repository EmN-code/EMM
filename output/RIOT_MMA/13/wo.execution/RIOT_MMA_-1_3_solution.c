#include "ztimer.h"
#include "thread.h"
#include "time_units.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "event/thread.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

#define I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS // Corrected I2C address
#define READ_INTERVAL (5U * ZTIMER_SEC) // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

void* read_accelerometer(void* arg) {
    (void)arg;
    // Initialize accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error initializing accelerometer");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accel X: %dmg, Y: %dmg, Z: %dmg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting accelerometer read loop");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_accelerometer, NULL, "read_accelerometer");
    return 0;
}