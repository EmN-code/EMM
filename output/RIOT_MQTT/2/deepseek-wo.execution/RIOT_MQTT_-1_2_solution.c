
#include <net/sock.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include "MQTTClient.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN 0

static void mqtt_message_arrived(MessageData* data) {
    (void)data;
}

int main(void) {
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote;
    sock_udp_t sock;
    int res;

    remote.family = AF_INET;
    remote.port = EMQX_SERVER_PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;

    res = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        puts("Error converting IP address");
        return 1;
    }

    res = sock_udp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    MQTTClient client;
    Network network;
    unsigned char sendbuf[200], readbuf[200];
    int rc = 0;

    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "RIOT_MQTT_CLIENT";
    data.keepAliveInterval = 20;
    data.cleansession = 1;

    if ((rc = MQTTConnect(&client, &data)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return 1;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = RETAIN;
        message.payload = PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("Failed to publish, return code %d\n", rc);
        }

        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
