#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "timex.h"
#include "stdatomic.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "time_units.h"
#include "unistd.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN GPIO_PIN(0, 2) // GPIO 2
#define MQTT_BROKER_IP "192.168.1.1"
#define MQTT_BROKER_PORT 1883

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *dht_thread(void *arg) {
    (void)arg;

    dht_t dev;
    int16_t temp, hum;
    int res = dht_init(&dev, &dht_params[0]);
    if (res != DHT_OK) {
        printf("DHT initialization failed\n");
        return NULL;
    }

    while (1) {
        res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT
");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

void *mqtt_thread(void *arg) {
    (void)arg;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    int rc;
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        } else {
            printf("Connected to MQTT Broker\n");
            break;
        }
    }

    // Placeholder for any publish/subscribe operations
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, dht_thread, NULL, "dht_thread");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}