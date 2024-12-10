
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 360
#define MQTT_TIMEOUT 10000

static Network network;
static MQTTClient mqttClient;

static int mqtt_read(Network *n, unsigned char *read_buffer, int len, int timeout)
{
    return sock_tcp_recv(n->sock, read_buffer, len, timeout);
}

static int mqtt_write(Network *n, unsigned char *send_buffer, int len, int timeout)
{
    return sock_tcp_send(n->sock, send_buffer, len, timeout);
}

static void mqtt_connect(void)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = MQTT_CLIENT_ID;
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = 1;

    int rc = MQTTConnect(&mqttClient, &data);
    if (rc != 0) {
        printf("MQTT connect failed with error code %d\n", rc);
    } else {
        printf("MQTT connected\n");
    }
}

static void mqtt_publish(void)
{
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void *)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    int rc = MQTTPublish(&mqttClient, MQTT_TOPIC, &message);
    if (rc != 0) {
        printf("MQTT publish failed with error code %d\n", rc);
    } else {
        printf("MQTT published\n");
    }
}

int main(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int sock = sock_tcp_create(&local, &remote, 0);
    if (sock < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    network.sock = sock;
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&mqttClient, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
