#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_ID          "board"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS          (16U)
#define TOPIC_MAXLEN       (64U)
#define MQTT_INTERVAL      (5U) // seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static char stack_pub[THREAD_STACKSIZE_DEFAULT];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static emcute_topic_t pub_topic;

static sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

static void *pub_thread(void *arg) {
    (void)arg;
    char *pub_data = "Work";

    while (1) {
        int res = emcute_pub(&pub_topic, pub_data, strlen(pub_data), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[pub] unable to publish data (%d)\n", res);
        } else {
            printf("[pub] published data: %s\n", pub_data);
        }
        // wait for specified interval (MQTT_INTERVAL)
        xtimer_sleep(MQTT_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_run, "emcute", "emcute");

    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", "47.102.103.1", 1883);
        return 1;
    }
    pub_topic.name = "state";

    // create separate thread for publishing
    thread_create(stack_pub, sizeof(stack_pub), THREAD_PRIORITY_MAIN - 1,
                  0, pub_thread, NULL, "pub_thread");

    return 0;
}