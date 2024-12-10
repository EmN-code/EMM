
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "net/sock/tcp.h"
#include "net/sock/util.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN 0

static sock_tcp_t sock;
static Network network;
static MQTTClient mqtt_client;

static int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&sock, buffer, len, 0);
}

static int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&sock, buffer, len, 0);
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAIN;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&mqtt_client, TOPIC, &message);
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = htons(BROKER_PORT);
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local) < 0) {
        puts("Error: Could not connect to MQTT broker");
        return 1;
    }

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqtt_client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Error: Could not connect to MQTT broker");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
