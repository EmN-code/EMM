#include "arpa/inet.h"
#include "stdlib.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "dht_params.h"
#include "xtimer.h"
#include "time_units.h"
#include "msg.h"
#include "thread.h"
#include "stdatomic.h"
#include "string.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"
#define STACK_SIZE 2048
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char stack[STACK_SIZE];

static MQTTClient client;
static Network network;

static unsigned char sendbuf[256], readbuf[256];

void *publish_state(void *arg) {
    (void)arg;
    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.id = 0;
        char payload[] = "work";
        message.payload = payload;
        message.payloadlen = strlen(payload);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) < 0) {
            printf("Error publishing message\n");
        } else {
            printf("Message published: %s\n", payload);
        }

        // Publish every 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    NetworkInit(&network);

    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Error connecting to network\n");
        return -1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32_Client";

    if (MQTTConnect(&client, &data) != 0) {
        printf("MQTT connect failed\n");
        return -1;
    } else {
        printf("MQTT connected\n");
    }

    // Create a thread to publish the state every 5 seconds
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, publish_state, NULL, "publish_state");

    return 0;
}