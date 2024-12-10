#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID "riot-esp32"

static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published to topic %s: %s\n", topic->name, (char*)data);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Failed to connect to broker: %d\n", res);
        return 1;
    }

    topic.name = "state";
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Failed to register topic: %d\n", res);
        return 1;
    }

    char *state = "work";

    while (1) {
        res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0); 
        if (res != EMCUTE_OK) {
            printf("Failed to publish: %d\n", res);
        }
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}