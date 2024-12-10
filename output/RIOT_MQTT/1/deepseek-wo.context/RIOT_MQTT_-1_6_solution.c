
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U) /* 5 seconds in microseconds */

static void network_init(Network *n)
{
    n->mqttread = sock_tcp_read;
    n->mqttwrite = sock_tcp_write;
}

static int network_connect(Network *n, const char *address, int port)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    sock_tcp_t sock;

    remote.family = AF_INET;
    remote.port = port;
    inet_pton(AF_INET, address, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, 0) < 0) {
        return -1;
    }

    n->sock = sock;
    return 0;
}

static void network_disconnect(Network *n)
{
    sock_tcp_close(&n->sock);
}

static void publish_state(MQTTClient *client)
{
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void *)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(client, TOPIC, &message);
}

int main(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    network_init(&network);
    if (network_connect(&network, BROKER_IP, BROKER_PORT) < 0) {
        puts("Failed to connect to broker");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        network_disconnect(&network);
        return 1;
    }

    while (1) {
        publish_state(&client);
        xtimer_usleep(INTERVAL);
    }

    MQTTDisconnect(&client);
    network_disconnect(&network);

    return 0;
}
