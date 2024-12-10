#include "ztimer.h"
#include "thread.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "MQTTPacket.h"
#include "MQTTClient.h"

// Wifi and MQTT broker credentials
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883

#define MQTT_TOPIC "state"
#define PUB_INTERVAL (5 * US_PER_SEC)

// Buffer sizes
#define MQTT_BUF_SIZE 128
#define MQTT_TIMEOUT 10000L

static unsigned char sendbuf[MQTT_BUF_SIZE], readbuf[MQTT_BUF_SIZE];

void *mqtt_thread(void *arg) {
    (void)arg;  // suppress unused parameter warning
    MQTTClient client;
    Network network;
    int rc;
    unsigned char connack_rc;
    int buflen = sizeof(sendbuf);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    data.keepAliveInterval = 20;
    data.cleansession = 1;
    data.clientID.cstring = "RIOT_ESP32_Client";

    if ((rc = MQTTConnect(&client, &data)) != SUCCESS) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return NULL;
    }

    while (1) {
        char *msg = "work";
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void *)msg;
        message.payloadlen = strlen(msg);

        if ((rc = MQTTPublish(&client, MQTT_TOPIC, &message)) != SUCCESS) {
            printf("Failed to publish, return code %d\n", rc);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

int main(void) {
    thread_create(mqtt_thread, THREAD_BUFFER_SIZE_MAIN, THREAD_PRIORITY_MAIN + 1,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING, mqtt_thread, NULL, "mqtt");

    printf("MQTT publisher running\n");

    while (1) {
        thread_yield();
    }

    return 0;
}
