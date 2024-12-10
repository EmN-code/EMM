
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <stdbool.h>
#include <timex.h>
#include <msg.h>
#include <event/thread.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100
#define TOPIC "state"
#define PAYLOAD "work"

static sock_udp_t sock;
static sock_udp_ep_t remote;
static Network network;
static MQTTClient client;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void publish_state(void) {
    MQTTMessage message;
    message.qos = 0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    // Initialize network
    NetworkInit(&network);

    // Initialize remote endpoint
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Connect to the network
    NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT);

    // Initialize MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    // Connect to MQTT broker
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    // Disconnect from MQTT broker
    MQTTDisconnect(&client);

    // Disconnect from network
    NetworkDisconnect(&network);

    return 0;
}
