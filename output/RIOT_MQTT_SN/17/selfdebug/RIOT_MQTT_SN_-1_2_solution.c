#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define IDLE_INTERVAL       (5U * US_PER_SEC)
#define MQTT_BROKER_ADDR    "47.102.103.1"
#define MQTT_BROKER_PORT    (1883U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static size_t num_subscriptions = 0;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, 8);
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

static int connect_and_publish(void) {
    sock_udp_ep_t gw = { .family = AF_INET6,
                         .port = MQTT_BROKER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_ADDR) == NULL) {
        puts("Error: unable to parse IPv6 address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }

    emcute_topic_t t;
    t.name = "state";
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    const char *data = "work";

    while (1) {
        if (emcute_pub(&t, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(IDLE_INTERVAL);
    }

    return 0;
}

int main(void) {
    puts("MQTT-SN publishing example");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    connect_and_publish();
    return 0;
}