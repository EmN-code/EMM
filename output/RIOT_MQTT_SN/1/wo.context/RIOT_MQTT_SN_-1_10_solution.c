#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <emcute.h>

#define EMCUTE_ID           "my_mqtt_sn_client"
#define EMCUTE_PORT         1883
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME          "device/state"
#define STATE_MESSAGE       "work"
#define SLEEP_DURATION      (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static int publish_state(const char *state) {
    return emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* Assuming a static IP address is assigned to the gateway */
    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, "47.102.103.1") == NULL) {
        puts("Invalid gateway address");
        return 1;
    }
    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    /* Connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to gateway");
        return 1;
    }
    puts("Connected to gateway");

    /* Register a topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Could not register topic");
        return 1;
    }

    while (1) {
        /* Publish the state */
        if (publish_state(STATE_MESSAGE) != EMCUTE_OK) {
            puts("Publish failed");
            return 1;
        }
        puts("State published");

        /* Sleep for the given duration */
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}