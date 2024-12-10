
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN 0

static MQTTClient client;
static Network network;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS;
    message.retained = RETAIN;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
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

    NetworkInit(&network);
    if (NetworkConnect(&network, EMQX_SERVER_IP, EMQX_SERVER_PORT) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    MQTTClientInit(&client, &network, 3000, NULL, 0, NULL, 0);
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if (MQTTConnect(&client, &data) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    MQTTClose(&client);
    NetworkDisconnect(&network);
    sock_udp_close(&sock);

    return 0;
}
