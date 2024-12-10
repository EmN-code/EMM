#include <stdatomic.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <mma8x5x_regs.h>
#include <paho_mqtt.h>
#include <stdbool.h>
#include <ztimer.h>
#include <msg.h>
#include <dht_params.h>
#include <timex.h>
#include <dht.h>
#include <stddef.h>
#include <stdio.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <stdlib.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <thread.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <event/thread.h>

static mma8x5x_t dev;
mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void connect_mqtt_network(Network *network) {
    int rc;
    while (1) {
        rc = NetworkConnect(network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }
}

void read_accelerometer(void) {
    if (mma8x5x_is_ready(&dev)) {
        mma8x5x_read(&dev, &data);
        printf("Accel X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    } else {
        printf("Accelerometer not ready\n");
    }
}

int main(void) {
    int res = mma8x5x_init(&dev, &params);
    if (res == MMA8X5X_OK) {
        printf("Accelerometer initialized successfully\n");
    } else {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    Network network;
    connect_mqtt_network(&network);

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}