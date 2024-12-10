#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "msg.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/paho_mqtt.h"
#include "net/gcoap.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"
#include "mqttclient.h"

#define BROKER_IP_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "state_topic"
#define MQTT_MESSAGE "work or not work"
#define SLEEP_INTERVAL 5

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t msg_queue[8];

void *mqtt_publish_thread(void *arg) {
    (void) arg;
    char send_buf[128], read_buf[128];
    Network network;
    MQTTClient client;

    NetworkInit(&network);

    // Connect to the network
    if (NetworkConnect(&network, BROKER_IP_ADDRESS, BROKER_PORT) != 0) {
        printf("Failed to connect to network\n");
        return NULL;
    }

    MQTTClientInit(&client, &network, 1000, send_buf, sizeof(send_buf), read_buf, sizeof(read_buf));

    // Connect to MQTT broker
    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.clientID.cstring = MQTT_CLIENT_ID;

    if (MQTTConnect(&client, &connect_data) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    printf("Connected to MQTT broker\n");

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.payload = (void *)MQTT_MESSAGE;
        message.payloadlen = strlen(MQTT_MESSAGE);

        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Message published: %s\n", MQTT_MESSAGE);
        }

        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT client starting\n");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mqtt_publish_thread, NULL, "mqtt_publish_thread");

    return 0;
}