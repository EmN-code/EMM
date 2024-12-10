
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

void mqtt_connection_lost(void *context, char *cause) {
    (void)context;
    (void)cause;
    puts("Connection to MQTT broker lost");
}

int mqtt_message_arrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    (void)context;
    (void)topicName;
    (void)topicLen;
    (void)message;
    return 1;
}

void mqtt_delivery_complete(void *context, MQTTClient_deliveryToken dt) {
    (void)context;
    (void)dt;
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, 2000, NULL, 0, NULL, 0);

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        MQTTPublish(&mqttClient, TOPIC, &message);

        xtimer_usleep(INTERVAL);
    }

    return 0;
}
