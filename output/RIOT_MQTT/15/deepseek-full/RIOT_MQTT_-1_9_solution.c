
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>
#include <mutex.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdatomic.h>
#include <stddef.h>
#include <sys/socket.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN false
#define MQTT_KEEPALIVE 20
#define MQTT_CLEANSESSION true
#define MQTT_TIMEOUT 10000

static Network network;
static MQTTClient client;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void network_connect(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);
    network.my_socket = sock.fd;
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = MQTT_CLEANSESSION;

    MQTTClientInit(&client, &network, MQTT_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    MQTTConnect(&client, &options);
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);

    MQTTPublish(&client, MQTT_TOPIC, &message);
}

void mqtt_disconnect(void) {
    MQTTDisconnect(&client);
}

int main(void) {
    network_connect();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    mqtt_disconnect();
    return 0;
}
