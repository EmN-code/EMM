
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time_units.h>
#include <stdbool.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAINED false
#define MQTT_KEEPALIVE 20
#define MQTT_CLEANSESSION true
#define MQTT_COMMAND_TIMEOUT 30000

typedef struct {
    int sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

int sock_tcp_read(Network *n, unsigned char* buffer, int len, int timeout) {
    return recv(n->sock, buffer, len, 0);
}

int sock_tcp_write(Network *n, unsigned char* buffer, int len, int timeout) {
    return send(n->sock, buffer, len, 0);
}

void NetworkInit(Network *n) {
    n->mqttread = sock_tcp_read;
    n->mqttwrite = sock_tcp_write;
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_number);
    inet_pton(AF_INET, address_ip, &server_addr.sin_addr);

    n->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (n->sock < 0) {
        return -1;
    }

    if (connect(n->sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        return -1;
    }

    return 0;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[200];
    unsigned char readbuf[200];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    if (NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEANSESSION;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAINED;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        sleep(5);
    }

    return 0;
}
