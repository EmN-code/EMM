#include <string.h>
#include <stdio.h>
#include <net/emcute>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_TOPIC "state"
#define SLEEP_INTERVAL 5
#define EMCUTE_PORT 1883

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Message published on topic '%s': %.*s\n", topic->name, (int)len, (char *)data);
}

int main(void) {
    char *topics[] = { PUB_TOPIC };

    /* Initialize EMCUTE */
    if (emcute_init(NULL, EMCUTE_PORT) != 0) {
        printf("Error: unable to initialize EMCUTE\n");
        return 1;
    }
    
    /* Connect to the MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0) != 0) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    /* Set up publishing topic */
    topic.name = topics[0];
    emcute_reg(&topic);

    /* Publishing loop */
    while (1) {
        const char *state = "work";  // or "not work"
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);

        /* Sleep for SLEEP_INTERVAL seconds */
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}