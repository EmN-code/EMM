#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "thread.h"
#include "stdio.h"
#include "pthread.h"
#include "stdint.h"
#include "msg.h"
#include "mutex.h"
#include "unistd.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"

#define I2C_DEV 0
#define MMA8X5X_RATE_50HZ 50
#define MMA8X5X_RANGE_2G 2
#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

static mma8x5x_t dev;
static mma8x5x_data_t data;
static MQTTClient client;

void *read_mma8x5x(void *arg) {
    (void) arg;
    
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize MMA8x5x
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Initialize the network and MQTT client
    Network network;
    int rc;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("Network connected successfully\n");
        break;
    }

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32_MMA8x5x";

    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }
        printf("MQTT connected successfully\n");
        break;
    }

    // Create a thread to read data every 5 seconds
    pthread_t mma8x5x_thread;
    pthread_create(&mma8x5x_thread, NULL, read_mma8x5x, NULL);

    // Wait for the thread to terminate
    pthread_join(mma8x5x_thread, NULL);

    return 0;
}