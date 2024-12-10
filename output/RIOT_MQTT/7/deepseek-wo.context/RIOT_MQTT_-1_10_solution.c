
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/sock/util.h>
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

static void network_init(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    sock_tcp_create(&network.sock, &local, &remote, 0);
}

static int network_connect(void) {
    return sock_tcp_connect(&network.sock, &remote, SOCK_NO_TIMEOUT);
}

static int network_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;  // Ignore timeout for simplicity
    return sock_tcp_read(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static int network_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;  // Ignore timeout for simplicity
    return sock_tcp_write(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

static void mqtt_init(void) {
    network_init();
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    network.mqttread = network_read;
    network.mqttwrite = network_write;
}

static void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = PAYLOAD_LEN;

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    mqtt_init();
    network_connect();
    MQTTConnect(&client, NULL);

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
