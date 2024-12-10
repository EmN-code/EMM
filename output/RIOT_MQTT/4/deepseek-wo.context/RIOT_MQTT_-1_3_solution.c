
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 30
#define MQTT_COMMAND_TIMEOUT_MS 3000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024
#define MQTT_PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static struct Network network;
static MQTTClient mqtt_client;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout; // Unused parameter
    return sock_tcp_recv(n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout; // Unused parameter
    return sock_tcp_send(n->sock, buffer, len, 0);
}

void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void)
{
    sock_tcp_ep_t broker_addr = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_addr.addr.ipv4);

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    if (sock_tcp_connect(&network.sock, &broker_addr, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    MQTTClientInit(&mqtt_client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return 0;
}
