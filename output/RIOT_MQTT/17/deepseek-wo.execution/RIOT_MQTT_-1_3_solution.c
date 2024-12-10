
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 1000

static sock_udp_t sock;
static MQTTClient client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

int network_read(struct Network* net, unsigned char* buffer, int len, int timeout)
{
    return sock_udp_recv(&sock, buffer, len, timeout, NULL);
}

int network_write(struct Network* net, unsigned char* buffer, int len, int timeout)
{
    return sock_udp_send(&sock, buffer, len, NULL);
}

void network_init(struct Network* net)
{
    net->mqttread = network_read;
    net->mqttwrite = network_write;
}

void network_connect(struct Network* net)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }
}

void network_disconnect(struct Network* net)
{
    sock_udp_close(&sock);
}

int main(void)
{
    struct Network network;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    network_init(&network);
    network_connect(&network);

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    network_disconnect(&network);
    return 0;
}
