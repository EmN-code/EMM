
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_COMMAND_TIMEOUT_MS 10000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024

typedef struct {
    sock_tcp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

int mqtt_network_read(Network* network, unsigned char* buffer, int len, int timeout) {
    sock_tcp_t *sock = &network->sock;
    return sock_tcp_read(sock, buffer, len, timeout);
}

int mqtt_network_write(Network* network, unsigned char* buffer, int len, int timeout) {
    sock_tcp_t *sock = &network->sock;
    return sock_tcp_write(sock, buffer, len, timeout);
}

void mqtt_network_init(Network* network) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    sock_tcp_create(&network->sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
    network->mqttread = mqtt_network_read;
    network->mqttwrite = mqtt_network_write;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
    unsigned char readbuf[MQTT_READ_BUF_SIZE];
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "riot_esp32";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    mqtt_network_init(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
