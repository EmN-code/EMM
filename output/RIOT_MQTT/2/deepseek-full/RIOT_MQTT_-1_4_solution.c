
#include <stddef.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <stdlib.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <msg.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <stdbool.h>
#include <xtimer.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000
#define MQTT_SEND_BUF_SIZE 100
#define MQTT_READ_BUF_SIZE 100

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

static int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_recv(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&n->sock, buffer, len);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("MQTT connect failed\n");
        return;
    }

    printf("MQTT connected\n");
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    if (MQTTPublish(&mqtt_client, MQTT_TOPIC, &message) != 0) {
        printf("MQTT publish failed\n");
        return;
    }

    printf("MQTT published\n");
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&mqtt_network.sock, &local, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    mqtt_network.mqttread = mqtt_read;
    mqtt_network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
