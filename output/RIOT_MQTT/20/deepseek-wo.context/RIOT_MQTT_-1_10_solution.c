
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/sock/udp.h"
#include "net/mqtt.h"
#include "net/mqtt_client.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient mqttClient;
static Network network;

void network_connect(Network *n, const char *address, int port) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = port };
    inet_pton(AF_INET, address, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);
    n->sock = (sock_tcp_t)sock;
}

void network_disconnect(Network *n) {
    sock_udp_close((sock_udp_t)n->sock);
}

int network_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;  // Ignoring timeout for simplicity
    return sock_udp_recv((sock_udp_t)n->sock, buffer, len, SOCK_NO_TIMEOUT, NULL);
}

int network_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;  // Ignoring timeout for simplicity
    return sock_udp_send((sock_udp_t)n->sock, buffer, len, NULL);
}

int main(void) {
    puts("MQTT client starting");

    // Initialize network
    network_connect(&network, BROKER_IP, BROKER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&mqttClient, &network, 1000, NULL, 0, NULL, 0);

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqttClient, &options) != 0) {
        puts("MQTT connect failed");
        return 1;
    }

    puts("MQTT connected");

    char payload[20];
    while (1) {
        // Publish state
        strcpy(payload, PAYLOAD);
        MQTTMessage message = {
            .qos = 0,
            .retained = 0,
            .dup = 0,
            .payload = payload,
            .payloadlen = strlen(payload)
        };

        if (MQTTPublish(&mqttClient, TOPIC, &message) != 0) {
            puts("MQTT publish failed");
        } else {
            puts("MQTT publish success");
        }

        xtimer_usleep(INTERVAL);
    }

    // Disconnect network
    network_disconnect(&network);

    return 0;
}
