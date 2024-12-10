#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("esp32-node")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define BROKER_PORT     (1883U)
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_INTERVAL    (5U * US_PER_SEC)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static const char *topics[] = {
    "workstate",
};

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;    // Should never be reached
}

static int publish_workstate(const char *message) {
    unsigned flags = EMCUTE_QOS_0;

    // publish the defined message
    if (emcute_pub(&topic, message, strlen(message), flags) != EMCUTE_OK) {
        printf("[error] unable to publish to topic '%s'\n", topic.name);
        return 1;
    }

    printf("[info] published message "%s" to topic "%s"\n", message, topic.name);
    return 0;
}

int main(void) {
    // initialize Emcute
    puts("[info] setting up broker connection...");

    // create the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    // allow time for network setup
    xtimer_sleep(3);

    // setup mqtt-sn broker connection
    sock_udp_ep_t gw = { .port = BROKER_PORT };
    ipv4_addr_from_str((char *)&gw.addr.ipv4, BROKER_ADDRESS);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[error] unable to connect to [%s]:%d\n", BROKER_ADDRESS, BROKER_PORT);
        return 1;
    }
    printf("[info] successfully connected to gateway at [%s]:%d\n", BROKER_ADDRESS, BROKER_PORT);

    // setup subscription topic
    topic.name = topics[0];
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[error] unable to obtain topic id for "%s"\n", topic.name);
        return 1;
    }

    printf("[info] subscribed to topic %s\n", topic.name);

    while (1) {
        publish_workstate("work");

        // suspend thread for PUB_INTERVAL
        xtimer_usleep(PUB_INTERVAL);
    }

    return 0;
}