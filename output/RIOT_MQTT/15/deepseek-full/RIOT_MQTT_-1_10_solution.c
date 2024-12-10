
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <xtimer.h>
#include <mutex.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define COMMAND_TIMEOUT_MS 10000
#define SEND_BUF_SIZE 100
#define READ_BUF_SIZE 100

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;
static MQTTClient client;
static Network network;
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char readbuf[READ_BUF_SIZE];

void init_network(void) {
    remote.family = AF_INET;
    remote.port = htons(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    sock_udp_create(&sock, &local, &remote, 0);
}

void init_mqtt(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, sendbuf, SEND_BUF_SIZE, readbuf, READ_BUF_SIZE);
}

void publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    init_network();
    init_mqtt();

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&client, &options) != 0) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
