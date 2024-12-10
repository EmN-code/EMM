#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>
#include <msg.h>
#include <emcute.h>

#define EMCUTE_ID           "esp32_client"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topicsubs[NUMOFSUBS][TOPIC_MAXLEN];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;  // should never be reached
}

static int publish(char *topic_name, char *msg) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = topic_name;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID");
        return 1;
    }

    if (emcute_pub(&t, msg, strlen(msg), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", topic_name);
        return 1;
    }

    printf("Published %s to topic %s\n", msg, topic_name);
    return 0;
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET6, "47.102.103.1", &gw.addr) == 0) {
        puts("error: unable to parse IP address");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    while (1) {
        publish("status", "work");
        xtimer_sleep(5);
        publish("status", "not work");
        xtimer_sleep(5);
    }

    return 0;
}