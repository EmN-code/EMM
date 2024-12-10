#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <thread.h>

#define EMCUTE_ID       ("mqtt-client")
#define EMCUTE_PORT     (1883U)
#define MQTT_TOPIC      ("work/state")
#define BROKER_IP       "47.102.103.1"
#define STATE_MSG       "work"
#define NON_STATE_MSG   "not work"
#define MSG_INTERVAL    (5U) // Publish every 5 seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static int publish_state(void) {
    int res;
    static bool work = false;

    work = !work;
    const char *state = work ? STATE_MSG : NON_STATE_MSG;

    res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        puts("[error] unable to publish data");
        return res;
    }

    printf("Published: %s\n", state);
    return EMCUTE_OK;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&(gw.addr.ipv6), BROKER_IP) == NULL) {
        puts("[error] unable to parse broker address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[error] unable to connect to broker");
        return NULL;
    }

    puts("Successfully connected to broker.");

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[error] unable to register topic");
        return NULL;
    }

    printf("Registered topic '%s' with ID %d\n", topic.name, topic.id);

    while (1) {
        xtimer_sleep(MSG_INTERVAL);
        publish_state();
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute thread");

    return 0;
}