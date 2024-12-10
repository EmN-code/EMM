
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static void init_network(void) {
    sock_udp_ep_t local = SOCK_UDP_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.id = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqttClient, TOPIC, &message);
}

int main(void) {
    init_network();

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_ESP32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqttClient, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
