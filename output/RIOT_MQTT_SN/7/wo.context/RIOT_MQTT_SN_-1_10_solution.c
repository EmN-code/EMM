#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/ipv6/addr.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <xtimer.h>

#define EMCUTE_ID           "riot-node"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         (1883U)
#define PUB_TOPIC_NAME      "state"
#define PUB_INTERVAL        (5U)

static char stack[THREAD_STACKSIZE_LARGE];
static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    // Initialize the emcute stack
    emcute_init();

    // Start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    // Transform broker address and store in gw
    if (inet_pton(AF_INET, BROKER_IP, &gw.addr) != 1) {
        puts("ERROR: unable to parse broker address");
        return 1;
    }

    // Connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
        puts("Connected to broker");
    } else {
        puts("ERROR: unable to connect to broker");
        return 1;
    }

    // Register the topic
    topic.name = PUB_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return 1;
    }
    
    // Publish every PUB_INTERVAL seconds
    while (1) {
        const char *pub_data = "work or not work";
        if (emcute_pub(&topic, pub_data, strlen(pub_data), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: unable to publish data");
            return 1;
        }
        printf("Published: %s\n", pub_data);
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}