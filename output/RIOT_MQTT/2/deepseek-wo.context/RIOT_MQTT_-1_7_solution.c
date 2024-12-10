
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include <net/af.h>
#include <net/sock/util.h>
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static Network network;
static MQTTClient mqttClient;

static int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(n->sock, buffer, len, 0);
}

static void connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network.sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    int rc = MQTTConnect(&mqttClient, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
    } else {
        printf("Connected to MQTT broker\n");
    }
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&mqttClient, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
