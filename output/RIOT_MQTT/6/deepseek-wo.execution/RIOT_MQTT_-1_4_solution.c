
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/sock.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <timex.h>
#include <xtimer.h>
#include <MQTTClient.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define KEEP_ALIVE 10

typedef struct {
    sock_udp_t sock;
    sock_udp_ep_t local;
    sock_udp_ep_t remote;
} Network;

void NetworkInit(Network *n) {
    n->local.family = AF_INET;
    n->remote.family = AF_INET;
    n->remote.port = SERVER_PORT;
}

int NetworkConnect(Network *n, char* address_ip, int port_number) {
    inet_pton(AF_INET, address_ip, &n->remote.addr.ipv4);
    return sock_udp_create(&n->sock, &n->local, &n->remote, 0);
}

void NetworkDisconnect(Network *n) {
    sock_udp_close(&n->sock);
}

int main(void) {
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    MQTTMessage message;

    NetworkInit(&network);
    if (NetworkConnect(&network, SERVER_IP, SERVER_PORT) < 0) {
        printf("Failed to connect to network\n");
        return 1;
    }

    unsigned char sendbuf[256];
    unsigned char readbuf[256];
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    connectData.clientID.cstring = CLIENT_ID;
    connectData.keepAliveInterval = KEEP_ALIVE;
    connectData.cleansession = 1;

    if (MQTTConnect(&client, &connectData) != 0) {
        printf("Failed to connect to MQTT broker\n");
        NetworkDisconnect(&network);
        return 1;
    }

    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = sizeof(PAYLOAD) - 1;

    while (1) {
        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            printf("Failed to publish message\n");
        }
        xtimer_sleep(5);
    }

    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
