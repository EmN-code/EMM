#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <xtimer.h>
#include <dht_params.h>
#include <dht.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MSG_BUFFER_SIZE 100
#define MQTT_CLIENT_STACK_SIZE (THREAD_STACKSIZE_DEFAULT + 512)

static char mqtt_client_stack[MQTT_CLIENT_STACK_SIZE];
static msg_t mqtt_client_queue[MSG_BUFFER_SIZE];

void mqtt_publish_state(MQTTClient *client) {
    MQTTMessage message;
    char *topic = "device/state";
    char *payload = "work";
    message.qos = QOS0;
    message.retained = false;
    message.payload = (void *)payload;
    message.payloadlen = strlen(payload);

    int rc = MQTTPublish(client, topic, &message);
    if (rc != 0) {
        printf("Error while publishing: %d\n", rc);
    }
}

void *mqtt_client_thread(void *arg) {
    MQTTClient client;
    Network network;
    unsigned char sendbuf[100], readbuf[100];

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.keepAliveInterval = 10;
    connectData.cleansession = 1;
    connectData.clientID.cstring = "ESP32Client";

    int rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    if (rc != 0) {
        printf("Unable to connect to MQTT broker: %d\n", rc);
        return NULL;
    }

    rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("MQTT connect failed: %d\n", rc);
        return NULL;
    }

    while (1) {
        mqtt_publish_state(&client);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    memset(mqtt_client_queue, 0, sizeof(mqtt_client_queue));
    thread_create(mqtt_client_stack, sizeof(mqtt_client_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_client_thread, NULL, "mqtt_client_thread");
    return 0;
}