#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_CLIENT_ID "riot-client"
#define BROKER_IP "47.102.103.1"
#define DEFAULT_PORT 1883

#define PUB_TOPIC_NAME "state"
#define PUB_INTERVAL (5U * US_PER_SEC)  // 5-second interval

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_broker_t broker;

static void publish_state(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = PUB_TOPIC_NAME;

    const char* state = "work";
    int res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return;
    }

    res = emcute_pub(&t, state, strlen(state), flags);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data (%d)\n", res);
    } else {
        puts("Published \"work\" to a topic\n");
    }
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("### received publication for topic '%s' [%i] ###\n", topic->name, (int)topic->id);
    printf("Data: %.*s\n", len, (char *)data);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(DEFAULT_PORT, EMCUTE_CLIENT_ID);
    return NULL;
}

int main(void) {
    ztimer_sleep(ZTIMER_MSEC, 1000);
    sock_udp_ep_t gw = { .family = AF_INET6, .port = DEFAULT_PORT };
    inet_pton(AF_INET6, BROKER_IP, &gw.addr.ipv6);

    memset(&broker, 0, sizeof(broker));
    broker.name = EMCUTE_ID;
    broker.gateway = gw;

    emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_QOS_0);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  emcute_thread, NULL, "emcute");

    while (1) {
        publish_state();
        ztimer_sleep(ZTIMER_USEC, PUB_INTERVAL);
    }

    return 0;
}