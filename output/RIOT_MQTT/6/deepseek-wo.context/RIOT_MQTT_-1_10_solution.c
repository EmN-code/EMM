
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static void network_connect(void) {
    int res = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (res != 0) {
        printf("Failed to connect to network\n");
        return;
    }
    printf("Network connected\n");
}

static void mqtt_connect(void) {
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    int res = MQTTConnect(&mqttClient, &data);
    if (res != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("MQTT connected\n");
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int res = MQTTPublish(&mqttClient, TOPIC, &message);
    if (res != 0) {
        printf("Failed to publish message\n");
        return;
    }
    printf("Message published\n");
}

int main(void) {
    printf("Starting MQTT client example\n");

    // Initialize network
    network.mqttread = sock_tcp_read;
    network.mqttwrite = sock_tcp_write;
    network.sock = SOCK_ADDR_ANY_NETIF;

    // Connect to network
    network_connect();

    // Initialize MQTT client
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    // Connect to MQTT broker
    mqtt_connect();

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
