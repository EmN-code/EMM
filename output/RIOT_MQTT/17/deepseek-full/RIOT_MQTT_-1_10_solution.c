
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <paho_mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 60

typedef struct {
    sock_tcp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

int mqtt_read(Network *n, unsigned char *buf, int len, int timeout) {
    return sock_tcp_read(&n->sock, buf, len, timeout);
}

int mqtt_write(Network *n, unsigned char *buf, int len, int timeout) {
    return sock_tcp_write(&n->sock, buf, len, timeout);
}

void NetworkInit(Network *n) {
    n->mqttread = mqtt_read;
    n->mqttwrite = mqtt_write;
}

int NetworkConnect(Network *n, char *address_ip, int port_number) {
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = port_number;
    inet_pton(AF_INET, address_ip, &remote.addr.ipv4);
    return sock_tcp_connect(&n->sock, &remote, SOCK_FLAGS_REUSE_EP);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    unsigned char sendbuf[256], readbuf[256];

    NetworkInit(&network);
    if (NetworkConnect(&network, BROKER_IP, BROKER_PORT) != 0) {
        puts("Failed to connect to broker");
        return 1;
    }

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }
        sleep(5);
    }

    return 0;
}
