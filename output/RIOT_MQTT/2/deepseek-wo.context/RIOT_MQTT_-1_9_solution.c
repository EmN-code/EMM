
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_tcp_t sock;
static Network network;
static MQTTClient client;

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&sock, buffer, len, 0);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&sock, buffer, len, 0);
}

void connect_to_broker(void)
{
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, NULL, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
