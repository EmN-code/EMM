
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include <event/thread.h>
#include <thread.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];
static event_queue_t mqtt_event_queue;

void mqtt_publish_task(void) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[256], readbuf[256];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_mqtt_client";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    int rc = inet_pton(AF_INET, MQTT_BROKER_IP, &network.addr.ipv4);
    if (rc != 1) {
        printf("Failed to convert IP address\n");
        return;
    }

    network.port = MQTT_BROKER_PORT;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    message.qos = QOS1;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    event_thread_init(&mqtt_event_queue, mqtt_thread_stack, sizeof(mqtt_thread_stack), MQTT_THREAD_PRIORITY);
    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), MQTT_THREAD_PRIORITY, THREAD_CREATE_STACKTEST, mqtt_publish_task, NULL, "mqtt_publish");

    return 0;
}
