#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <net/emcute.h>

#define EMCUTE_PORT       (1883U)
#define EMCUTE_ID         "mqtt_sn_sender"
#define TOPIC_NAME        "esp32_state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Received publication on topic "%s": %.*s\n", topic->name, (int)len, (char *)data);
}

static emcute_sub_t subscriptions[] = {
    { topic: { .name = TOPIC_NAME }, cb = _on_pub }
};

static void _publish_state(emcute_topic_t *topic) {
    const char *states[] = { "work", "not work" };
    static int idx = 0;
    
    idx = (idx + 1) % 2;  // alternating states
    int res = emcute_pub(topic, states[idx], strlen(states[idx]), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", res);
    } else {
        printf("Published: "%s" on topic "%s"\n", states[idx], topic->name);
    }
}

static void *_thread(void *arg) {
    (void)arg;
    while (1) {
        _publish_state(&topic);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    emcute_conf_t conf = {
        (char *)EMCUTE_ID,
        NULL,
        subscriptions,
        sizeof(subscriptions) / sizeof(subscriptions[0])
    };
    emcute_init(&conf);

    /* Set up our subscription topics */
    unsigned flags = EMCUTE_QOS_0;
    topic.name = TOPIC_NAME;
    if (emcute_sub(&topic, flags) != EMCUTE_OK) {
        printf("Error: unable to subscribe to topic "%s"\n", TOPIC_NAME);
        return 1;
    }

    /* Connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) == 0) {
        printf("error: unable to parse IP address\n");
        return 1;
    }
    if (emcute_con(&gw) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return 1;
    }
    printf("Connected to the MQTT-SN broker at %s:%u\n", "47.102.103.1", ECMUTE_PORT);

    /* Launch publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, _thread, NULL, "mqtt_sn_sender");

    return 0;
}