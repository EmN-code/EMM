
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_MESSAGE_LEN 4
#define INTERVAL_MS (5 * 1000000U)  // 5 seconds in microseconds

typedef struct {
    sock_tcp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

int mqtt_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_read(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

int mqtt_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout;
    return sock_tcp_write(&n->sock, buffer, len, SOCK_NO_TIMEOUT);
}

void connect_to_broker(Network *net) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&net->sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: Could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(Network *net) {
    unsigned char message[MQTT_MESSAGE_LEN];
    memcpy(message, MQTT_MESSAGE, MQTT_MESSAGE_LEN);

    if (net->mqttwrite(net, message, MQTT_MESSAGE_LEN, 0) < 0) {
        puts("Error: Could not publish message");
    } else {
        puts("State published");
    }
}

int main(void) {
    Network net;
    net.mqttread = mqtt_read;
    net.mqttwrite = mqtt_write;

    connect_to_broker(&net);

    while (1) {
        publish_state(&net);
        xtimer_usleep(INTERVAL_MS);
    }

    return 0;
}
