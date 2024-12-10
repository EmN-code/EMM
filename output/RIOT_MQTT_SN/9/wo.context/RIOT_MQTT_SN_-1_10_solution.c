#include <ztimer.h>
#include <stdio.h>
#include <kernel_types.h>
#include <msg.h>
#include <net/emcute.h>

#define SLEEP_DURATION_MS 5000
#define EMCUTE_PORT       1883
#define EMCUTE_ID         "esp32"
#define BROKER_IP         "47.102.103.1"
#define TOPIC_NAME        "state"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published state successfully\n");
}

static int pub_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("error: unable to publish data\n");
        return 1;
    }
    return 0;
}

int main(void) {
    static char topic_name[] = TOPIC_NAME;
    static emcute_sub_t subscriptions[1];

    subscriptions[0].topic.name = topic_name;
    subscriptions[0].cb = on_pub;

    /* Initialize our topic */
    topic.name = topic_name;

    /* Connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    /* Register the topic */
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic\n");
        return 1;
    }

    /* Publish state every 5 seconds */
    while (1) {
        pub_state("work");
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION_MS);
        pub_state("not work");
        ztimer_sleep(ZTIMER_MSEC, SLEEP_DURATION_MS);
    }

    return 0;
}