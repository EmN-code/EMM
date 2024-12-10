
#include "net/ipv4/addr.h"
#include "net/sock/tcp.h"
#include "net/sock/util.h"
#include "ztimer.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define TIMEOUT 10000L

static void network_init(Network *network) {
    network->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (network->socket < 0) {
        puts("Failed to create socket");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(network->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        puts("Failed to connect to server");
        return;
    }

    network->mqttread = (int (*)(Network*, unsigned char*, int, int))recv;
    network->mqttwrite = (int (*)(Network*, unsigned char*, int, int))send;
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[256];
    unsigned char readbuf[256];

    network_init(&network);

    MQTTClientInit(&client, &network, TIMEOUT, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.MQTTVersion = 3;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = 20;
    options.cleansession = 1;

    if (MQTTConnect(&client, &options) != 0) {
        puts("Failed to connect to MQTT broker");
        return 1;
    }

    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        if (MQTTPublish(&client, TOPIC, &message) != 0) {
            puts("Failed to publish message");
        }

        xtimer_usleep(5 * US_PER_SEC);
    }

    return 0;
}
