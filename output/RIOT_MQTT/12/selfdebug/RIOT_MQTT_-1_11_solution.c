#include <xtimer.h>
#include <stdio.h>
#include <net/sock/tcp.h>
#include <net/sock.h>
#include <net/mqttsn.h>

#define SERVER_IP   "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static int mqtt_connect(Network *network) {
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
        .sin_addr.s_addr = inet_addr(SERVER_IP),
    };

    network->sock.tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (network->sock.tcp < 0) {
        perror("Failed to create socket");
        return -1;
    }

    if (connect(network->sock.tcp, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to connect");
        return -1;
    }

    return 0;
}

static void publish_state(Network *network, const char *state) {
    char topic[] = "/state";
    MQTTMessage message;

    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(network, topic, &message);
}

int main(void) {
    Network network;
    MQTTClient client;
    unsigned char sendbuf[80], readbuf[80];
    const char *state = "work";

    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    if (mqtt_connect(&network) != 0) {
        puts("Connection to the MQTT broker failed");
        return 1;
    }

    while (1) {
        publish_state(&network, state);
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }

    return 0;
}