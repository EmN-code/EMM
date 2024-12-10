
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <xtimer.h>
#include <msg.h>
#include <MQTTClient.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_PAYLOAD "work"
#define MQTT_QOS 0
#define MQTT_RETAINED false
#define MQTT_KEEP_ALIVE 20
#define MQTT_CLEAN_SESSION true

static MQTTClient client;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void mqtt_publish(void) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void *)MQTT_PAYLOAD;
    pubmsg.payloadlen = sizeof(MQTT_PAYLOAD) - 1;
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = MQTT_RETAINED;

    MQTTClient_publishMessage(&client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(&client, token, 1000L);
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;

    remote.family = AF_INET;
    remote.port = HTONS(EMQX_SERVER_PORT);
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    MQTTClient_create(&client, "tcp://47.102.103.1:1883", "RIOT_MQTT_CLIENT", MQTTCLIENT_PERSISTENCE_NONE, NULL);

    connectData.keepAliveInterval = MQTT_KEEP_ALIVE;
    connectData.cleansession = MQTT_CLEAN_SESSION;

    if (MQTTConnect(&client, &connectData) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    MQTTClient_disconnect(&client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
