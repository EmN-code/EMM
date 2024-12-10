
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>  // Include this header for inet_pton
#include <net/emcute.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC_NAME "state"
#define CLIENT_ID "esp32_client"
#define PAYLOAD "work"

static emcute_topic_t topic;

void publish_state(void) {
    const char *payload = (strcmp(PAYLOAD, "work") == 0) ? "not work" : "work";
    printf("Publishing state: %s\n", payload);

    int res = emcute_pub(&topic, payload, strlen(payload), 0);
    if (res != EMCUTE_OK) {
        printf("Error publishing message: %d\n", res);
    }
}

int main(void) {
    sock_udp_ep_t gw = { .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    printf("Connecting to MQTT-SN broker at %s:%d\n", SERVER_IP, SERVER_PORT);
    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error connecting to MQTT-SN broker: %d\n", res);
        return 1;
    }

    printf("Registering topic: %s\n", TOPIC_NAME);
    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error registering topic: %d\n", res);
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
