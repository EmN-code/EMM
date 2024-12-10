
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/ipv4/addr.h>
#include <net/ipv6/addr.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <timex.h>
#include <stdatomic.h>
#include <unistd.h>
#include <time_units.h>
#include <event/thread.h>
#include <stdbool.h>
#include <MQTTClient.h>
#include <msg.h>
#include <thread.h>
#include <net/sock/util.h>
#include <stddef.h>
#include <sys/socket.h>
#include <ztimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEPALIVE 60

typedef struct {
    sock_tcp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} Network;

Network network;
MQTTClient client;

int mqtt_read(Network* n, unsigned char* buffer, int len, int timeout) {
    return sock_tcp_read(&n->sock, buffer, len, timeout);
}

int mqtt_write(Network* n, unsigned char* buffer, int len, int timeout) {
    return sock_tcp_write(&n->sock, buffer, len, timeout);
}

void network_init(Network* n) {
    n->mqttread = mqtt_read;
    n->mqttwrite = mqtt_write;
}

int network_connect(Network* n, const char* address_ip, int port_number) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote;
    remote.family = AF_INET;
    remote.port = port_number;
    inet_pton(AF_INET, address_ip, &remote.addr.ipv4);

    return sock_tcp_connect(&n->sock, &remote, &local, 0);
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = sizeof(PAYLOAD) - 1;

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    network_init(&network);
    if (network_connect(&network, BROKER_IP, BROKER_PORT) < 0) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    unsigned char sendbuf[100];
    unsigned char readbuf[100];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_mqtt_client";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
