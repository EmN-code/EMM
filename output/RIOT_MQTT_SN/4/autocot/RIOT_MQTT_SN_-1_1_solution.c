#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32_client"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)

#define TOPIC_NAME      "device/state"
#define BROKER_IP       "47.102.103.1"

#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[1];
static char subscription_buffer[64U];

static emcute_topic_t topic;
static mqttsn_pubsub_t client;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("Published on topic %s [%.*s]\n", topic->name, (int)len, (char *)data);
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6 };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_IP);
    gw.port = EMCUTE_PORT;

    if (emcute_con(&client, EMCUTE_ID, &gw, EMCUTE_PRIO) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }
    return 0;
}

static void *publisher_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *state_msg = "not work";
        emcute_topic_t t;
        unsigned flags = EMCUTE_QOS_0;
        if (some_condition()) {  // Replace with your own condition logic
            state_msg = "work";
        }
        t.name = TOPIC_NAME;
        t.id = 0;  // Publish without prior subscription

        if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return NULL;
}

int main(void) {
    // Initialize emcute
    memset(subscriptions, 0, sizeof(subscriptions));
    emcute_init(stack, sizeof(stack), EMCUTE_PORT);

    // Connect to the MQTT-SN broker
    if (connect_broker() != 0) {
        return 1;
    }

    // Start the publisher thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  publisher_thread, NULL, "publisher");

    // Main loop can be used for other purposes if required
    while (1) {
        xtimer_sleep(PUBLISH_INTERVAL);
    }
    return 0;
}