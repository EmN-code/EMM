#include "xtimer.h"
#include "ztimer.h"
#include "periph/rtc.h"
#include "sys/socket.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "msg.h"
#include "net/gcoap.h"
#include "timex.h"
#include "stddef.h"
#include "esp_wifi.h"
#include "unistd.h"
#include "mma8x5x_params.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "time_units.h"
#include "MQTTClient.h"
#include "dht.h"
#include "stdlib.h"
#include "mutex.h"
#include "mmaxb5x.h"
#include "stdio.h"
#include "kernel_defines.h"
#include "periph_conf.h"
#include "arpa/inet.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "pthread.h"
#include "esp_system.h"
#include "evtimer_msg.h"
#include "stdatomic.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

// Define device, parameters, and data structures
static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Function to read data from the accelerometer
void read_accelerometer(void) {
    int res = mma8x5x_read(&dev, &data);
    if (res == 0) {
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Failed to read from accelerometer\n");
    }
}

// Thread function for periodic reading
void *thread_handler(void *arg) {
    (void) arg;
    while (1) {
        read_accelerometer();
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Initialize the MMA8xxxx sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }
    printf("Accelerometer initialized\n");

    // Create a thread for continuously reading accelerometer data
    char stack[THREAD_STACKSIZE_MAIN];
    thread_t thread;
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, thread_handler, NULL, "accelerometer_thread");

    // Main thread does nothing, just returns
    return 0;
}