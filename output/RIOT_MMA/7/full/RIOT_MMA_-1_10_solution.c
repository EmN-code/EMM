#include <xtimer.h>
#include <unistd.h>
#include <thread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>
#include <netinet/in.h>
#include <msg.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <dht_params.h>
#include <dht.h>
#include <event/thread.h>
#include <mbox.h>
#include <timex.h>
#include <architecture.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <ztimer.h>
#include <time_units.h>
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_system.h"
#include "esp_wifi.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor_data(void *arg) {
    (void)arg;
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Sensor not ready\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    puts("Initializing MMA8x5x sensor");
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_sensor_data, NULL, "read_sensor_data");

    return 0;
}