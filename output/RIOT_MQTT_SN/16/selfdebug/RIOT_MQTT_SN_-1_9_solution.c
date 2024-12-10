#include "xtimer.h"
#include "net/ipv4/addr.h"
#include "net/emcute.h"
#include "kernel_defines.h"

#define EMCUTE_ID       "device"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT     (1883U)
#define EMCUTE_SERVER_IP "47.102.103.1"
#define PUB_INTERVAL    (5U * US_PER_SEC)

static emcute_sub_t subscriptions[CONFIG_EMCUTE_SUBSCRIPTIONS_MAX];
static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void on_publish(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET, EMCUTE_SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN gateway");
        return NULL;
    }

    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        const char *state_msg = "work";
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish message");
        }
        xtimer_sleep(PUB_INTERVAL / US_PER_SEC);
    }

    return NULL;
}

int main(void) {  
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, "emcute", emcute_thread, NULL);
    return 0;
}