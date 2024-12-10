#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <net/emcute_stub.h>

#define EMCUTE_ID           "ger"  
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

// Function to connect to MQTT-SN broker
static int connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };
    if (ipv6_addr_from_str((ipv6_addr_t *) &gw.addr.ipv6, "47.102.103.1") == NULL) {
        printf("Error: unable to parse IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Unable to connect to gateway\n");
        return 1;
    }
    puts("Successfully connected to gateway\n");
    return 0;
}

// Function to publish message
static void publish_message(void) {
    char *work_state = "work or not work";
    unsigned flags = EMCUTE_QOS_0;  // QoS level 0
    if (emcute_pub(&topic, work_state, strlen(work_state), flags) != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
    } else {
        printf("Published: %s\n", work_state);
    }
}

int main(void) {
    // Initialize emcute
    emcute_run(THREAD_CREATE_STACKTEST, stack, sizeof(stack), EMCUTE_PRIO, "emcute", queue, 8);

    // Connect to broker
    if (connect_to_broker() != 0) {
        return 1;
    }

    // Register topic
    topic.name = "workstate";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }

    // Periodically publish a message every 5 seconds
    while (1) {
        publish_message();
        xtimer_sleep(5);
    }

    return 0;
}