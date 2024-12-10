#include <net/ipv6/addr.h>
#include <net/gnrc.h>
#include <msg.h>
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       "mqtt_client"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC     "state"
#define STATE_PAYLOAD   "work"

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[8];

static void on_pub(const emcute_topic_t *topic, 
                   void *data, size_t len) {
    printf("Received publication on topic "%s": %.*s\n", 
           topic->name, (int)len, (char *)data);
}

static int publish(const char *topic, const char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }
    flags |= (qos & EMCUTE_QOS_MASK);

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic "%s"\n", topic);
        return 1;
    }

    printf("published %s on topic %s\n", data, topic);
    return 0;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, 
                  "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    while (1) {
        publish(STATE_TOPIC, STATE_PAYLOAD, EMCUTE_QOS_0);
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}