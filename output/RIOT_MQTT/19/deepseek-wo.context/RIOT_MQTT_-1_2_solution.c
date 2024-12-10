
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

void network_init(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT, .addr = BROKER_IP };

    sock_udp_create(&network.sock, &local, &remote, 0);

    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;
}

void mqtt_init(void) {
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    int rc = MQTTConnect(&mqttClient, &data);
    if (rc != 0) {
        printf("MQTT connect failed\n");
    } else {
        printf("MQTT connected\n");
    }
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int rc = MQTTPublish(&mqttClient, TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed\n");
    } else {
        printf("MQTT published\n");
    }
}

int main(void) {
    network_init();
    mqtt_init();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
