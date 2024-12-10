#include <netinet/in.h>
#include <dht_params.h>
#include <ztimer.h>
#include <xtimer.h>
#include <kernel_defines.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <timex.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <dht.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <mbox.h>
#include <stdatomic.h>
#include <MQTTClient.h>
#include <periph_conf.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <mma8x5x_regs.h>

#define MMA8X5X_I2C_ADDRESS       (0x1D)  // Default I2C address
#define MMA8X5X_RATE_50HZ         (50)    // Data rate
#define MMA8X5X_RANGE_2G          (2)     // Full scale range

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *mma8x5x_thread(void *arg) {
    (void)arg;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        usleep(5000000); // Sleep for 5 seconds without using xtimer
    }

    return NULL;
}

int main(void) {
    char thread_stack[THREAD_STACKSIZE_MAIN];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_thread, NULL, "mma8x5x");

    return 0;
}