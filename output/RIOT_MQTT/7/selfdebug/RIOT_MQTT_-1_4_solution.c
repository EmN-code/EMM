#include <stdlib.h>
#include <string.h>
#include <net/mqttsn.h>
#include <net/sock/udp.h>
#include <stdio.h>
#include <xtimer.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL 5

static int publish_state(sock_udp_ep_t *remote) {
    const char *topic = "device/state";
    const char *message = "work";
    size_t message_len = strlen(message);
    int res = mqttsn_pub(remote, &message, message_len, 1);
    if (res < 0) {
        printf("Error publishing message\n");
    }
    return res;
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: invalid MQTT broker address\n");
        return -1;
    }

    while (1) {
        publish_state(&remote);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}