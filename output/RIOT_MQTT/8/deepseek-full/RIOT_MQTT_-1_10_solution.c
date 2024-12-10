
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <thread.h>
#include <mutex.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT_MS 30000
#define SEND_BUF_SIZE 1024
#define READ_BUF_SIZE 1024

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;

void NetworkInit(Network *n) {
    n->my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (n->my_socket < 0) {
        puts("Failed to create socket");
        return;
    }
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    remote.port = htons(EMQX_SERVER_PORT);
    remote.family = AF_INET;
    if (connect(n->my_socket, (struct sockaddr *)&remote.addr, sizeof(remote.addr)) < 0) {
        puts("Failed to connect to MQTT broker");
        return;
    }
}

int MQTTPublish(MQTTClient* client, const char* topic, MQTTMessage* message) {
    // Implement the publish logic here
    return 0;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[SEND_BUF_SIZE];
    unsigned char readbuf[READ_BUF_SIZE];
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, MQTT_COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);

    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    puts("Connected to MQTT broker");

    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    while (1) {
        if (MQTTPublish(&client, MQTT_TOPIC, &message) != 0) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    return 0;
}
