#include <thread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mutex.h>
#include <net/emcute.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define BROKER_PORT         1883
#define MQTT_TOPIC          "state"
#define PUBLISH_INTERVAL    (5U)    // Publish interval in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;
static char pub_buf[64];

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .port = BROKER_PORT };
    gw.family = AF_INET6;

    // Set the broker IP
    ipv6_addr_set_all_nodes_multicast(&gw.addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker");
        return NULL;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        emcute_discon();
        return NULL;
    }

    while (1) {
        snprintf(pub_buf, sizeof(pub_buf), "Current state: working");
        if (emcute_pub(&topic, pub_buf, strlen(pub_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish");
        }
        thread_sleep(PUBLISH_INTERVAL * US_PER_SEC);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN example
");

    // Initialize the emcute stack
    emcute_run(EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO, queue, ARRAY_SIZE(queue));

    // Start the MQTT-SN thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt_thread");

    return 0;
}