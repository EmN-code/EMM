
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <timex.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000
#define MQTT_SEND_BUF_SIZE 1024
#define MQTT_READ_BUF_SIZE 1024

static char sendbuf[MQTT_SEND_BUF_SIZE];
static char readbuf[MQTT_READ_BUF_SIZE];

static Network network;
static MQTTClient mqttClient;

static void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";

    if (MQTTConnect(&mqttClient, &options) != 0) {
        printf("MQTT connect failed\n");
        return;
    }
    printf("MQTT connected\n");
}

static void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_MESSAGE;
    message.payloadlen = strlen(MQTT_MESSAGE);

    if (MQTTPublish(&mqttClient, MQTT_TOPIC, &message) != 0) {
        printf("MQTT publish failed\n");
        return;
    }
    printf("MQTT message published\n");
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;

    local.port = 0;
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        printf("Invalid IP address\n");
        return 1;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    NetworkInit(&network);
    MQTTClientInit(&mqttClient, &network, MQTT_COMMAND_TIMEOUT, sendbuf, MQTT_SEND_BUF_SIZE, readbuf, MQTT_READ_BUF_SIZE);

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
    return 0;
}
