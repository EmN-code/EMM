
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/inet_pton.h>
#include <net/sock/tcp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define MQTT_YIELD_POLLING_MS (30)

static sock_tcp_t mqtt_sock;
static MQTTClient mqtt_client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

static int mqtt_network_read(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    (void)n;
    return sock_tcp_read(&mqtt_sock, buffer, len, timeout_ms);
}

static int mqtt_network_write(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    (void)n;
    return sock_tcp_write(&mqtt_sock, buffer, len, timeout_ms);
}

static void mqtt_connect(void)
{
    Network mqtt_network;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    mqtt_network.mqttread = mqtt_network_read;
    mqtt_network.mqttwrite = mqtt_network_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_YIELD_POLLING_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_mqtt_client";
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("MQTT connect failed\n");
    } else {
        printf("MQTT connected\n");
    }
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
        printf("MQTT publish failed\n");
    } else {
        printf("MQTT published\n");
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        printf("Failed to create TCP socket\n");
        return 1;
    }

    if (sock_tcp_connect(&mqtt_sock, &remote, SOCK_NO_TIMEOUT) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
