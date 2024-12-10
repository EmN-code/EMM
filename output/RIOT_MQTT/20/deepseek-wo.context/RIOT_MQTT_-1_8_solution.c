
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEANSESSION 1
#define MQTT_TIMEOUT 10000
#define MQTT_BUFFER_SIZE 1024
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

static char mqtt_thread_stack[MQTT_THREAD_STACKSIZE];
static msg_t mqtt_thread_msg_queue[8];

static void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(client, MQTT_TOPIC, &message);
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEANSESSION;
    options.MQTTVersion = 3;

    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        mqtt_publish_state(&client);
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    return NULL;
}

int main(void) {
    msg_init_queue(mqtt_thread_msg_queue, sizeof(mqtt_thread_msg_queue) / sizeof(msg_t));

    thread_create(mqtt_thread_stack, sizeof(mqtt_thread_stack), MQTT_THREAD_PRIORITY,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}
