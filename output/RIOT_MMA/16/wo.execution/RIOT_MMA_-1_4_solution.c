#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "msg.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "time_units.h"
#include "dht_params.h"
#include "MQTTClient.h"
#include "mbox.h"

#define I2C_DEV_PARAM        I2C_DEV(0)
#define MMA8X5X_ADDR         (0x1D)  // example I2C address; replace with your specific address if different
#define MMA8X5X_RATE         (2)     // example rate setting; replace as needed
#define MMA8X5X_RANGE        (0)     // example range setting; replace as needed

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV_PARAM,
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE,
    .range = MMA8X5X_RANGE
};
static mma8x5x_data_t data;

void *reader_thread(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8x5x device\n");
        return 1;
    }
    printf("Initialized the MMA8x5x device\n");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, reader_thread, NULL, "reader_thread");

    return 0;
}