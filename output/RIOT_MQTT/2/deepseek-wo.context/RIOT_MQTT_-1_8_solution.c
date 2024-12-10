
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "riot_esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_tcp_t sock;
static Network network;
static MQTTClient client;

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_read(&n->sock, buffer, len, 0);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_write(&n->sock, buffer, len, 0);
}

void connect_to_broker(void) {
    int result;
    struct sock_tcp_ep remote;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    result = sock_tcp_create(&network.sock, NULL, &remote, SOCK_FLAGS_REUSE_EP);
    if (result < 0) {
        printf("Failed to create socket\n");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    result = MQTTConnect(&client, &options);
    if (result != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
    printf("Published: %s\n", PAYLOAD);
}

int main(void) {
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
