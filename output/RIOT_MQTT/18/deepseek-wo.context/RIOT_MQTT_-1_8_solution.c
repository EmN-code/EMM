
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void network_read(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;  // unused parameter
    sock_tcp_recv(n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static void network_write(Network *n, unsigned char *buffer, int len, int timeout)
{
    (void)timeout;  // unused parameter
    sock_tcp_send(n->sock, buffer, len);
}

static void network_disconnect(Network *n)
{
    sock_tcp_close(n->sock);
}

static int network_connect(Network *n, const char *addr, int port)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = port;
    inet_pton(AF_INET, addr, &remote.addr.ipv4);

    return sock_tcp_connect(&n->sock, &remote, &local, 0);
}

int main(void)
{
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    unsigned char sendbuf[100], readbuf[100];

    // Initialize the network
    network.mqttread = network_read;
    network.mqttwrite = network_write;
    network.sock = SOCK_TCP_NULL;

    // Connect to the MQTT broker
    if (network_connect(&network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Set up the connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    // Connect to the MQTT broker
    if (MQTTConnect(&client, &connectData) != 0) {
        puts("Failed to connect to MQTT broker");
        network_disconnect(&network);
        return 1;
    }

    puts("Connected to MQTT broker");

    // Publish state every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = 0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }

        xtimer_usleep(INTERVAL);
    }

    // Disconnect from the MQTT broker
    network_disconnect(&network);

    return 0;
}
