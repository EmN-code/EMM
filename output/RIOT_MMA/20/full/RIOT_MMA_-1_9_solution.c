#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <unistd.h>
#include <ztimer.h>
#include <xtimer.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <msg.h>
#include <mutex.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <dht_params.h>
#include <dht.h>
#include <periph_conf.h>
#include <periph_i2c.h>
#include <periph_gpio.h>
#include <periph/rtc.h>
#include <thread.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define I2C_ADDR                (0x1D << 1)  // Example I2C address for MMA8x5x

static char stack[THREAD_STACKSIZE_MAIN];

static void *read_mma8x5x(void *arg) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration -> X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    thread_create(stack, THREAD_STACKSIZE_MAIN,
                  THREAD_PRIORITY_MAIN, 0,
                  read_mma8x5x, NULL, "read_mma8x5x");

    return 0;
}