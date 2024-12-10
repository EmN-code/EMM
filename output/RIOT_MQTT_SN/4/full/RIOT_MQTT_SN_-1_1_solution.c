#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME "state"
#define PUB_INTERVAL (5U * US_PER_SEC) // Publish every 5 seconds

static emcute_topic_t topic;
static char message[32];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Published data of length %zu\n", len);
}

int mqttsn_init(void) {
    emcute_conf_t conf;
    conf.id = "mqttsn-client";
    conf.port = BROKER_PORT;

    emcute_init(&conf);

    topic.name = TOPIC_NAME;
    topic.id = 0;

    int ret = emcute_reg(&topic);
    if (ret != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return ret;
    }
    return EMCUTE_OK;
}

void *publish(void *arg) {
    (void)arg;
    while (1) {
        sprintf(message, "work");  // Message content can be modified
        emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    if (mqttsn_init() != EMCUTE_OK) {
        printf("MQTT-SN initialization failed\n");
        return -1;
    }

    puts("MQTT-SN client connected\n");

    publish(NULL);
    return 0;
}