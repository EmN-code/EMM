#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "dht_params.h"
#include "timex.h"
#include "dht.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "architecture.h"
#include "mutex.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "xtimer.h"
#include "mma8x5x_params.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

static pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;  

static void *read_mma8x5x(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    while (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x, retrying...\n");
        xtimer_sleep(1);
    }
    printf("MMA8x5x initialized successfully!\n");

    while (1) {
        pthread_mutex_lock(&data_mutex);
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", data.x_axis, data.y_axis, data.z_axis);
        pthread_mutex_unlock(&data_mutex);
        xtimer_sleep(5);
    }

    return NULL;
}

static void *connect_mqtt(void *arg) {
    (void)arg;
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer; 
    int rc;

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }

        rc = MQTTConnect(&client, &connectData);
        if (rc != 0) {
            printf("MQTT connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        }
        break;
    }

    printf("MQTT connected successfully!\n");

    return NULL;
}

int main(void) {
    pthread_t mma_thread, mqtt_thread;

    pthread_create(&mma_thread, NULL, read_mma8x5x, NULL);
    pthread_create(&mqtt_thread, NULL, connect_mqtt, NULL);

    pthread_join(mma_thread, NULL);
    pthread_join(mqtt_thread, NULL);

    return 0;
}