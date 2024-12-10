#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <emcute.h>
#include <xtimer.h>

#define BROKER_ADDR   "47.102.103.1"
#define BROKER_PORT   1883
#define TOPIC_NAME    "device/state"

static emcute_topic_t topic;

void on_mqtt_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    char *msg = (char *)data;
    printf("Received: %s\n", msg);
}

int main(void) {
    // Network setup
    char client_id[] = "esp32_client";
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDR, &gw.addr.ipv4);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Could not connect to the broker %s\n", BROKER_ADDR);
        return 1;
    }

    // Topic registration
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Could not register topic %s\n", TOPIC_NAME);
        return 1;
    }

    // Publishing loop
    while (1) {
        char *state = (xtimer_now_usec() % 2 == 0) ? "work" : "not work";
        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Publish failed\n");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_sleep(5);
    }

    return 0;
}