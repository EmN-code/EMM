#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "MQTTClient.h"

// Define broker address and port
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC) // 5 seconds

static sock_udp_ep_t remote;
static MQTTClient client;
static unsigned char sendbuf[128], recvbuf[128];

void publish_state(const char *state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.payload = (void *)state;
    message.payloadlen = strlen(state);

    MQTTPublish(&client, "state", &message);
}

int main(void) {
    // Initialize network address
    ipv6_addr_from_str(&remote.addr.ipv6, BROKER_IP);
    remote.port = BROKER_PORT;

    // Initialize MQTTClient
    Network network;
    NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf));

    // Connect to the broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_ESP32";
    MQTTConnect(&client, &connectData);

    char *state = "work";
    while (1) {
        publish_state(state);
        if (strcmp(state, "work") == 0) {
            state = "not work";
        } else {
            state = "work";
        }
        xtimer_sleep(PUBLISH_INTERVAL / US_PER_SEC);
    }
    return 0;
}