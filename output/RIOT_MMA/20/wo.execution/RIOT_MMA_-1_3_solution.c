#include "xtimer.h"
#include "time_units.h"
#include "thread_config.h"
#include "thread.h"
#include "timex.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"

#define I2C_DEV           (I2C_DEV(0))
#define MMA8X5X_ADDR      CONFIG_MMA8X5X_I2C_ADDRESS
#define SAMPLE_RATE_MS    (5000U)  // 5 seconds

static mma8x5x_t accel_device;
static mma8x5x_data_t accel_data;

void *sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        int res = mma8x5x_read(&accel_device, &accel_data);
        if (res == MMA8X5X_OK) {
            printf("Accel X: %d, Y: %d, Z: %d\n", accel_data.x, accel_data.y, accel_data.z);
        }
        else {
            printf("Failed to read accelerometer\n");
        }
        xtimer_sleep(SAMPLE_RATE_MS / 1000);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&accel_device, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  sensor_thread, NULL, "sensor_thread");

    return 0;
}