
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
#include <MQTTClient.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 1000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024

static MQTTClient mqtt_client;
static Network mqtt_network;
static unsigned char sendbuf[MQTT_SEND_BUF_SIZE];
static unsigned char readbuf[MQTT_READ_BUF_SIZE];

void mqtt_publish_state(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&mqtt_client, &options) != 0) {
        printf("MQTT connection failed\n");
        return;
    }

    printf("MQTT connected\n");
}

void mqtt_init(void) {
    NetworkInit(&mqtt_network);
    MQTTClientInit(&mqtt_client, &mqtt_network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 0;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_SERVER_PORT };
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&mqtt_network.my_socket, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return;
    }

    mqtt_connect();
}

int main(void) {
    mqtt_init();

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
