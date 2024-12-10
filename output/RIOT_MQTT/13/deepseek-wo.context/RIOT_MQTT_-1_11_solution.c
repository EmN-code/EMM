
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void network_init(Network *n) {
    n->mqttread = NULL;
    n->mqttwrite = NULL;
}

static int network_connect(Network *n, const char *addr, int port) {
    // Implement network connection logic here
    // This is a placeholder for the actual network connection code
    return 0;
}

static void publish_state(MQTTClient *client) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    network_init(&network);
    if (network_connect(&network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Failed to connect to network");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
