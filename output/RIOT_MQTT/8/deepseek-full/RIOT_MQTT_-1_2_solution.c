
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <sys/socket.h>
#include "MQTTClient.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_CLEAN_SESSION 1

static MQTTClient client;
static Network network;
static unsigned char sendbuf[200], readbuf[200];

void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    int sock;
    struct sockaddr_in broker_addr;
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = htons(MQTT_BROKER_PORT);

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) <= 0) {
        puts("Invalid address/ Address not supported");
        return 1;
    }

    sock = sock_udp_create(&local, &remote, 0);
    if (sock < 0) {
        puts("Failed to create UDP socket");
        return 1;
    }

    NetworkInit(&network);
    network.my_socket = sock;
    network.mqttread = sock_udp_recv;
    network.mqttwrite = sock_udp_send;

    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = MQTT_KEEPALIVE;
    data.cleansession = MQTT_CLEAN_SESSION;

    if (MQTTConnect(&client, &data) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    while (1) {
        mqtt_publish_state();
        sleep(5);
    }

    MQTTDisconnect(&client);
    sock_udp_close(sock);

    return 0;
}
