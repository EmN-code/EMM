#include <ztimer.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "net/emcute.h"
#include "kernel.h"

#define EMCUTE_PORT          (1883U)
#define EMCUTE_ID            "esp32"
#define TOPIC_NAME           "device/state"
#define MQTT_SN_BROKER_IP    "47.102.103.1"
#define SLEEP_INTERVAL       (5U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    /* Do nothing, just a stub callback */
}

static void *publish_thread(void *arg) {
    (void)arg;
    char *states[] = {"work", "not work"};
    size_t state_idx = 0;

    while (1) {
        xtimer_sleep(SLEEP_INTERVAL);
        char *state_msg = states[state_idx];

        emcute_msg_t msg;
        msg.data = state_msg;
        msg.len = strlen(state_msg);
        msg.flags = EMCUTE_MSG_FLAG_DUP;

        if (emcute_pub(&topic, &msg) != EMCUTE_OK) {
            printf("Failed to publish message\n");
        }
        else {
            printf("Published: %s\n", state_msg);
        }

        /* toggle between work and not work */
        state_idx = 1 - state_idx;
    }
    return NULL;
}

static int setup_connection(void) {
    sock_udp_ep_t gw = { .family = AF_INET,
                         .netif = SOCK_ADDR_ANY_NETIF,
                         .port = EMCUTE_PORT };   

    if (ipv6_addr_from_str(&gw.addr.ipv6, MQTT_SN_BROKER_IP) == NULL) {
        puts("Error: unable to parse MQTT-SN broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    /* setup subscription structure */
    topic.name = TOPIC_NAME;

    if (emcute_sub(&topic, on_pub, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic\n");
        return 1;
    }

    return 0;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish");

    if (setup_connection() != 0) {
        return 1;
    }

    puts("MQTT-SN client setup complete\n");

    /* Stay in the main thread to keep the program running */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}
