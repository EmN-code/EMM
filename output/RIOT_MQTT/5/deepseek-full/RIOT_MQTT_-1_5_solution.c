
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <xtimer.h>
#include <msg.h>
#include <mutex.h>
#include <stdbool.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN 0

static sock_udp_t sock;
static sock_udp_ep_t server;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_client";

    int rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return;
    }
    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAIN;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    int rc = MQTTPublish(&client, TOPIC, &message);
    if (rc != 0) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Message published\n");
    }
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    server.family = AF_INET;
    server.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &server.addr.ipv4);

    int rc = NetworkConnect(&network, SERVER_IP, SERVER_PORT);
    if (rc != 0) {
        printf("Failed to connect to network, return code %d\n", rc);
        return 1;
    }

    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
