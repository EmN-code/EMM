#include <ztimer.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dht.h>
#include <dht_params.h>
#include <msg.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <thread.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <architecture.h>

#define DHT_PIN             GPIO_PIN(0, 2)
#define DHT_TYPE            DHT11
#define SLEEP_DURATION      (5U)
#define MQTT_BROKER_IP      "192.168.0.10"
#define MQTT_BROKER_PORT    1883
#define MQTT_PUBLISH_TOPIC  "dht11/data"

static dht_t dev;
static MQTTClient client;
static char sendbuf[100];
static char readbuf[100];

void init_dht(void) {
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;

    int result = dht_init(&dev, &params);
    if (result != DHT_OK) {
        printf("DHT initialization failed\n");
        exit(1);
    }
}

int connect_to_mqtt_broker(void) {
    Network network;
    int rc;
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "RIOT_DHT_Client";

    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if(rc != 0) {
            printf("[MQTT] Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        rc = MQTTConnect(&client, &connect_data);
        if (rc != 0) {
            printf("[MQTT] Connection to broker failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            continue;
        }

        printf("[MQTT] Connected to broker\n");
        break;
    }
    return rc;
}

void publish_data(int temp, int hum) {
    MQTTMessage message;
    char payload[30];
    snprintf(payload, sizeof(payload), "Temp: %d C, Hum: %d%%", temp, hum);
    message.payload = payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;

    int rc = MQTTPublish(&client, MQTT_PUBLISH_TOPIC, &message);
    if (rc != 0) {
        printf("[MQTT] Publish failed\n");
    } else {
        printf("[MQTT] Message published successfully\n");
    }
}

static void *dht11_thread(void *arg) {
    (void)arg;
    int temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("[DHT11] Temp: %d C, Hum: %d%%\n", temp, hum);
            publish_data(temp, hum);
        } else {
            printf("[DHT11] Reading failed\n");
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_DURATION);
    }
    return NULL;
}

int main(void) {
    init_dht();
    connect_to_mqtt_broker();

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, dht11_thread, NULL, "dht11");

    return 0;
}