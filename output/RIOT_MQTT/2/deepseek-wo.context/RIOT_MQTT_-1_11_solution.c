
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t sock;
static MQTTClient mqtt_client;
static unsigned char sendbuf[100];
static unsigned char readbuf[100];

void network_init(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = BROKER_PORT;
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating TCP socket");
        return;
    }

    if (sock_tcp_connect(&sock, &remote, 0) < 0) {
        puts("Error connecting to broker");
        return;
    }
}

void network_disconnect(void) {
    sock_tcp_disconnect(&sock);
    sock_tcp_close(&sock);
}

int network_read(Network *network, unsigned char *buffer, int len, int timeout) {
    (void)timeout; // unused parameter
    return sock_tcp_read(&sock, buffer, len, 0);
}

int network_write(Network *network, unsigned char *buffer, int len, int timeout) {
    (void)timeout; // unused parameter
    return sock_tcp_write(&sock, buffer, len, 0);
}

int main(void) {
    Network network;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = CLIENT_ID;
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    network_init();

    network.mqttread = network_read;
    network.mqttwrite = network_write;

    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        network_disconnect();
        return 1;
    }

    puts("Connected to MQTT broker");

    while (1) {
        MQTTMessage message;
        message.qos = QOS0;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = PAYLOAD_LEN;

        if (MQTTPublish(&mqtt_client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }

        xtimer_usleep(INTERVAL);
    }

    network_disconnect();
    return 0;
}
