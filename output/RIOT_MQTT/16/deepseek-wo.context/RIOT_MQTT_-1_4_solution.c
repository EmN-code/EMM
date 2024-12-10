
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static MQTTClient client;

void connect_to_broker(void) {
    Network n;
    NetworkInit(&n);
    int result = NetworkConnect(&n, SERVER_IP, SERVER_PORT);
    if (result != 0) {
        puts("Failed to connect to the broker");
        return;
    }

    MQTTClientInit(&client, &n, 1000, NULL, 0, NULL, 0);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_ESP32";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    result = MQTTConnect(&client, &data);
    if (result != 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }
    puts("Connected to MQTT broker");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    int result = MQTTPublish(&client, TOPIC, &message);
    if (result != 0) {
        puts("Failed to publish message");
    } else {
        puts("Message published successfully");
    }
}

int main(void) {
    puts("Starting MQTT client on ESP32");

    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
