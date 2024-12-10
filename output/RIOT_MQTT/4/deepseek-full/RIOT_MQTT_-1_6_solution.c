
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <mutex.h>
#include <MQTTClient.h>

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static Network network;
static MQTTClient client;

void network_init(void) {
    NetworkInit(&network);
}

int connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    return MQTTConnect(&client, &options);
}

int publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    return MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init();

    unsigned char sendbuf[200], readbuf[200];
    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (connect_to_broker() != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        if (publish_state() != 0) {
            printf("Failed to publish state\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
