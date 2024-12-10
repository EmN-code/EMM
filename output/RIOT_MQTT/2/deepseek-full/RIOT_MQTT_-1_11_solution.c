
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/ipv4/addr.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 5000000

static int mqtt_read(struct Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_read(&n->sock, buffer, len);
}

static int mqtt_write(struct Network* n, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return sock_tcp_send(&n->sock, buffer, len);
}

static void connect_to_broker(struct Network* network)
{
    sock_tcp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = HTONS(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&network->sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: Could not connect to server");
        return;
    }
}

int main(void)
{
    struct Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    network.mqttread = mqtt_read;
    network.mqttwrite = mqtt_write;

    connect_to_broker(&network);

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);

    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Error: Could not connect to MQTT broker");
        return 1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Error: Could not publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_usleep(TIMEOUT);
    }

    MQTTDisconnect(&client);
    sock_tcp_close(&network.sock);

    return 0;
}
