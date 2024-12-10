#include "ztimer.h"
#include "xtimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "esp_system.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"

#define INTERVAL (5 * 1000000U) // 5 second interval in microseconds
#define MQTT_BROKER_IP "192.168.1.100"
#define MQTT_BROKER_PORT 1883

void *mma8x5x_read_task(void *arg) {
    // Initialize accelerometer params
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize and check accelerometer
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Accelerometer initialization failed\n");
        return NULL;
    }

    mma8x5x_data_t data;

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);
        printf("Accel: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        usleep(INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Network and MQTT client setup
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    int rc;

    // Attempt to establish network connection
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed\n");
            usleep(1000000); // Wait 1 second before retrying
        } else {
            printf("Connected to network\n");
            break;
        }
    }

    // Initialize MQTT client
    unsigned char sendbuf[80], readbuf[80];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    while (1) {
        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed\n");
            usleep(1000000); // Wait 1 second before retrying
        } else {
            printf("MQTT connected\n");
            break;
        }
    }

    // Start accelerometer read task
    thread_create(NULL, 1024, 0, 0, mma8x5x_read_task, NULL, "mma8x5x_task");

    return 0;
}