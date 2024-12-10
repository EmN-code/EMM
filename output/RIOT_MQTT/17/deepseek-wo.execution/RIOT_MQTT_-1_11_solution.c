
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <net/ipv4/addr.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_CLIENT_ID "riot_esp32"
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 60

static sock_tcp_t sock;
static Network network;
static MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout)
{
    return sock_tcp_read(&sock, buffer, len, timeout);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout)
{
    return sock_tcp_write(&sock, buffer, len, timeout);
}

void connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        sleep(5);
    }

    return 0;
}
