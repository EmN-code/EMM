#include <esp_wifi.h>
#include <esp_system.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mutex.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <periph_gpio.h>
#include <periph_conf.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <thread.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_accel_data(void *arg) {
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x, data.y, data.z);
        usleep(5000000);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G,
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];

    kernel_pid_t pid = thread_create(stack, sizeof(stack),
                                     THREAD_PRIORITY_MAIN - 1,
                                     0, read_accel_data, NULL, "accel_reader");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    while (1) {}
    return 0;
}