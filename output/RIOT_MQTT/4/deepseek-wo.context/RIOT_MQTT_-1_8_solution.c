
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define MQTT_YIELD_POLLING_MS 30

static sock_tcp_t sock;
static MQTTClient mqtt_client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

static int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    (void)timeout_ms;
    return sock_tcp_read(&sock, buffer, len, 0);
}

static int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout_ms)
{
    (void)timeout_ms;
    return sock_tcp_write(&sock, buffer, len, 0);
}

static void mqtt_network_init(Network *n)
{
    n->mqttread = mqtt_read;
    n->mqttwrite = mqtt_write;
}

static void mqtt_connect(Network *n)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        puts("Error: failed to connect to MQTT broker");
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = MQTT_CLIENT_ID;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    MQTTConnect(&mqtt_client, &options);
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

int main(void)
{
    Network network;
    mqtt_network_init(&network);
    MQTTClientInit(&mqtt_client, &network, MQTT_YIELD_POLLING_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect(&network);

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
