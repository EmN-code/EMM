
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <kernel_defines.h>
#include <timex.h>
#include <time_units.h>
#include <mutex.h>
#include <msg.h>
#include <mbox.h>
#include <thread.h>
#include <stdatomic.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <MQTTClient.h>

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define STACK_SIZE 1024
#define PRIORITY 5

static mma8x5x_t dev;
static mma8x5x_data_t data;
static msg_t msg_queue[8];

void read_accelerometer(void *arg) {
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
        sleep(5);
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    char stack[STACK_SIZE];
    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, read_accelerometer, NULL, "accelerometer_reader");
    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}
