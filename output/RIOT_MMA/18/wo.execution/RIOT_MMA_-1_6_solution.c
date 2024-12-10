#include "xtimer.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "msg.h"
#include "ztimer.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "dht.h"
#include "dht_params.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "timex.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "mqttpaho.h"
#include "MQTTClient.h"

#define SLEEP_TIME 5

typedef struct {
    int16_t x, y, z;
} mma8x5x_data_t;

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_sleep(SLEEP_TIME);
    }
    return NULL;
}

int main(void) {
    printf("Starting MMA8x5x accelerometer reader...\n");
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x!\n");
        return 1;
    }
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_accelerometer, NULL, "read_accelerometer");
    return 0;
}
