#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/sock/tcp.h"
#include "MQTTClient.h"

#define SLEEP_SEC 5
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_PUB_MSG "work"
#define MQTT_BUFFER_SIZE 256

static sock_tcp_t sock;
static uint8_t sendbuf[MQTT_BUFFER_SIZE];
static int should_stop = 0;

void initialize_network(Network *network) {
    NetworkInit(network);
    int ret = NetworkConnect(network, BROKER_IP, BROKER_PORT);
    if (ret != SUCCESS) {
        printf("Failed to connect to network\n");
    }
}

void publish_state(MQTTClient *client, char *message) {
    MQTTMessage mqtt_message;
    mqtt_message.qos = QOS0;
    mqtt_message.retained = 0;
    mqtt_message.payload = (void *)message;
    mqtt_message.payloadlen = strlen(message);
    
    int ret = MQTTPublish(client, MQTT_TOPIC, &mqtt_message);
    if (ret != SUCCESS) {
        printf("Failed to publish message\n");
    }
}

void application_thread(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    
    initialize_network(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, MQTT_BUFFER_SIZE, sendbuf, MQTT_BUFFER_SIZE);

    int ret = MQTTConnect(&client, &connectData);
    if (ret != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
    }

    while (!should_stop) {
        publish_state(&client, MQTT_PUB_MSG);
        xtimer_sleep(SLEEP_SEC);
    }

    NetworkDisconnect(&network);
}

int main(void) {
    application_thread();
    return 0;
}