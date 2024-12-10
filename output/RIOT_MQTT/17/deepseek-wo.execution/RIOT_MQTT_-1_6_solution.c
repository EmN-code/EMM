
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static sock_tcp_t sock;

int mqtt_read(struct Network* network, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return recv(network->sock, buffer, len, 0);
}

int mqtt_write(struct Network* network, unsigned char* buffer, int len, int timeout)
{
    (void)timeout;
    return send(network->sock, buffer, len, 0);
}

void network_init(struct Network* network)
{
    network->mqttread = mqtt_read;
    network->mqttwrite = mqtt_write;
}

int main(void)
{
    struct Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;

    local.port = 0;
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: Could not connect to server");
        return 1;
    }

    network_init(&network);
    network.sock = sock;

    MQTTClientInit(&client, &network, TIMEOUT, NULL, 0, NULL, 0);
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";

    if (MQTTConnect(&client, &connectData) != 0) {
        puts("Error: Could not connect to MQTT broker");
        return 1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = sizeof(PAYLOAD) - 1;

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Error: Could not publish message");
        } else {
            puts("Message published successfully");
        }
        xtimer_sleep(5);
    }

    return 0;
}
