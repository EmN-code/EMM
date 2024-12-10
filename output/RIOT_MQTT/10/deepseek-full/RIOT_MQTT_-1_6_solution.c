
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define TIMEOUT 10000L

static sock_udp_t sock;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[2048];
static unsigned char readbuf[2048];

void connect_to_broker(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = htons(BROKER_PORT) };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return;
    }

    network.mqttread = (int (*)(Network*, unsigned char*, int, int))sock_udp_recv;
    network.mqttwrite = (int (*)(Network*, unsigned char*, int, int))sock_udp_send;

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "riot_client";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        puts("Error connecting to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&client, TOPIC, &message) != 0) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

void periodic_publish(void) {
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }
}

int main(void) {
    connect_to_broker();
    periodic_publish();
    return 0;
}
