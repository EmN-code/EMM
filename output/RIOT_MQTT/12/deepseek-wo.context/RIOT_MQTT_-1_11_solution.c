
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static Network network;
static MQTTClient client;

static int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&n->sock, buffer, len, 0);
}

static int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_write(&n->sock, buffer, len, 0);
}

static void connect_to_broker(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_tcp_t sock;
    sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);

    network.sock = sock;
    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_ESP32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

static void publish_state(void)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published: %s\n", PAYLOAD);
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
