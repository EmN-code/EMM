#include "ztimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "sys/socket.h"
#include "timex.h"
#include "unistd.h"
#include "xtimer.h"
#include "architecture.h"
#include "arpa/inet.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "mutex.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "paho_mqtt.h"
#include "thread.h"
#include "time_units.h"

#define GPIO_PIN 2

static dht_t dev;

int main(void) {
    int16_t temp, hum;
    int rc, retries;
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];

    // Initialize sensor
    gpio_init(GPIO_PIN, GPIO_OUT);
    rc = dht_init(&dev, &dht_params[0]);
    if (rc != DHT_OK) {
        printf("Error initializing DHT sensor\n");
        return 1;
    }

    // Initialize network and MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != 0) {
            printf("Failed to connect to network, reconnecting...\n");
            xtimer_sleep(1);
            continue;
        }
        printf("Connected successfully\n");
        break;
    }

    while (1) {
        rc = dht_read(&dev, &temp, &hum);
        if (rc != DHT_OK) {
            printf("Failed to read DHT sensor\n");
        } else {
            printf("Temperature: %2.1f°C, Humidity: %2.1f%%\n", temp / 10.0, hum / 10.0);
        }
        xtimer_sleep(5);
    }
    return 0;
}