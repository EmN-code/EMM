
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/mqttsn.h"
#include "MQTTClient.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static MQTTClient client;
static Network network;
static MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

void publish_state(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)PAYLOAD;
    message.payloadlen = strlen(PAYLOAD);

    MQTTPublish(&client, TOPIC, &message);
}

int main(void) {
    // Initialize the network
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &remote.addr.ipv4);

    sock_udp_t sock;
    sock_udp_create(&sock, &local, &remote, 0);

    // Initialize the MQTT client
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 30000, NULL, 0, NULL, 0);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT_MQTT_CLIENT";
    connectData.keepAliveInterval = 20;
    connectData.cleansession = 1;

    int rc = MQTTConnect(&client, &connectData);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return 1;
    }

    // Set up periodic timer for publishing state
    xtimer_t timer;
    xtimer_init(&timer);
    xtimer_set_periodic(&timer, INTERVAL, (void (*)(void *))publish_state, NULL);

    // Main loop
    while (1) {
        MQTTYield(&client, 1000);
        xtimer_usleep(100000);  // Sleep for 100ms
    }

    return 0;
}
