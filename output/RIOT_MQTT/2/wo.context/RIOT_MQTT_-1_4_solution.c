#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <MQTTClient.h>  // Ensure 'MQTTClient.h' is the last included

#define MQTT_BROKER_ADDR "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define PUBLISH_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static sock_udp_ep_t broker_ep = {
    .family = AF_INET,
    .addr.ipv4 = IPV4_ADDR_ANY,
    .port = MQTT_BROKER_PORT
};

static MQTTClient client;
static Network network;

void mqtt_publish_state(void) {
    const char *states[2] = {"work", "not work"};
    static size_t counter = 0;
    MQTTMessage message;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)states[counter];
    message.payloadlen = strlen(states[counter]);

    MQTTPublish(&client, MQTT_TOPIC, &message);
    counter = (counter + 1) % 2;
}

int main(void) {
    printf("MQTT simple publisher example
");

    // Initialize UDP endpoint
    broker_ep.family = AF_INET;
    inet_pton(AF_INET, MQTT_BROKER_ADDR, &broker_ep.addr.ipv4);

    // Initialize network
    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_ADDR, MQTT_BROKER_PORT) != SUCCESS) {
        printf("Failed to connect to network
");
        return 1;
    }

    // Initialize MQTT client
    unsigned char sendbuf[100], readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData connect_data = MQTTPacket_connectData_initializer;
    connect_data.MQTTVersion = 3;
    connect_data.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&client, &connect_data) != SUCCESS) {
        printf("Failed to connect to MQTT broker
");
        return 1;
    }

    // Publish state at regular intervals
    while (1) {
        mqtt_publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}