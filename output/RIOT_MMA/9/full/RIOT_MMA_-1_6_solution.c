#include <ztimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdio.h>

#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "msg.h"

#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#include "paho_mqtt.h"
#include "dht_params.h"
#include "dht.h"

#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include "kernel_defines.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_DEFAULT 1024
#define THREAD_PRIORITY_MAIN 1
#define MMA8X5X_I2C_ADDRESS 0x1D  // Assuming the I2C address

typedef struct {
    int16_t x_axis;
    int16_t y_axis;
    int16_t z_axis;
} mma8x5x_data_t;

void *read_mma8x5x(void *arg) {
    mma8x5x_t dev;
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

    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);  // Simplified function call
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, read_mma8x5x, NULL, "mma8x5x_thread");

    Network network;
    MQTTClient client;
    int rc;

    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != SUCCESS) {
            printf("Network connection failed, retrying...\n");
            sleep(1);
            continue;
        }
        printf("Connected to network\n");
        break;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    while (1) {
        rc = MQTTConnect(&client);
        if (rc != SUCCESS) {
            printf("MQTT connection failed, retrying...\n");
            sleep(1);
            continue;
        }
        printf("Connected to MQTT\n");
        break;
    }

    // Additional logic to manage subscriptions, sending data, etc.
    return 0;
}