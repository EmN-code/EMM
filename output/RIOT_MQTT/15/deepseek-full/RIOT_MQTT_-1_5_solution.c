
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define BUF_SIZE 1024
#define TOPIC "state"
#define PAYLOAD "work"

static sock_udp_t sock;
static sock_udp_ep_t remote;
static MQTTClient mqtt_client;
static Network network;
static unsigned char sendbuf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

void connect_to_broker(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_client";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }
    printf("Connected to MQTT broker\n");
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    if (MQTTPublish(&mqtt_client, TOPIC, &message) != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published message: %s\n", PAYLOAD);
    }
}

int main(void) {
    // Initialize network
    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    // Initialize MQTT client
    MQTTClientInit(&mqtt_client, &network, 1000, sendbuf, BUF_SIZE, readbuf, BUF_SIZE);

    // Connect to MQTT broker
    connect_to_broker();

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
