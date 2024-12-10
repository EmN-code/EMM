#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mbox.h"
#include "unistd.h"
#include "xtimer.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define MMA8X5X_I2C_ADDRESS 0x1D  // Adjust according to actual address
#define MQTT_BROKER_IP  "192.168.1.1"  // Placeholder IP address
#define MQTT_BROKER_PORT 1883  // Placeholder port

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor_data() {
    if (mma8x5x_is_ready(&dev)) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        printf("Sensor not ready\n");
    }
}

int main(void) {
    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Initialize network
    Network network;
    while (1) {
        int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected to the network successfully\n");
        break;
    }

    printf("Starting main loop\n");
    while (1) {
        read_sensor_data();
        ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5 seconds
    }
    return 0;
}