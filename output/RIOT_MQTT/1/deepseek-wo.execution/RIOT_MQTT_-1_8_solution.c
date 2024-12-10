
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <msg.h>
#include <arpa/inet.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work or not work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_TIMEOUT 1000

#define MSG_QUEUE_SIZE 10

static msg_t msg_queue[MSG_QUEUE_SIZE];

static void network_init(Network *n) {
    n->mqttread = NULL;
    n->mqttwrite = NULL;
}

static int network_connect(Network *n, const char *address_ip, int port_number) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = port_number };

    if (inet_pton(AF_INET, address_ip, &remote.addr.ipv4) <= 0) {
        return -1;
    }

    if (sock_udp_create(&n->sock, &local, &remote, 0) < 0) {
        return -1;
    }

    return 0;
}

static void network_disconnect(Network *n) {
    sock_udp_close(&n->sock);
}

static int network_read(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout; // Ignore timeout for simplicity
    return sock_udp_recv(&n->sock, buffer, len, SOCK_NO_TIMEOUT, NULL);
}

static int network_write(Network *n, unsigned char *buffer, int len, int timeout) {
    (void)timeout; // Ignore timeout for simplicity
    return sock_udp_send(&n->sock, buffer, len, NULL);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    network_init(&network);
    if (network_connect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) < 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    MQTTClientInit(&client, &network, MQTT_TIMEOUT, NULL, 0, NULL, 0);

    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        network_disconnect(&network);
        return 1;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void *)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    network_disconnect(&network);

    return 0;
}
