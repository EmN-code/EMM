
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define MQTT_COMMAND_TIMEOUT 30000

static sock_udp_t sock;
static sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
static sock_udp_ep_t remote;
static MQTTClient client;
static unsigned char sendbuf[256];
static unsigned char readbuf[256];

void network_init(void) {
    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    sock_udp_create(&sock, &local, &remote, 0);
}

void mqtt_init(void) {
    NetworkInit(&client.ipstack);
    MQTTClientInit(&client, &client.ipstack, MQTT_COMMAND_TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
}

void mqtt_connect(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.clientID.cstring = "riot_esp32";
    MQTTConnect(&client, &options);
}

void mqtt_publish(void) {
    MQTTMessage message;
    message.qos = MQTT_QOS;
    message.retained = MQTT_RETAIN;
    message.dup = 0;
    message.payload = (void*)MQTT_PAYLOAD;
    message.payloadlen = strlen(MQTT_PAYLOAD);
    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    network_init();
    mqtt_init();
    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    return 0;
}
