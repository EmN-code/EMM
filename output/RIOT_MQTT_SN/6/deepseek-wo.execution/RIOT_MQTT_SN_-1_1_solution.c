
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/sock/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static void publish_state(void)
{
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    char payload[32];
    sprintf(payload, "%s", states[state_index]);
    state_index = (state_index + 1) % 2;

    emcute_pub_t pub;
    pub.topic = &topic;
    pub.qos = EMCUTE_QOS_0;
    pub.retain = 0;
    pub.data = payload;
    pub.data_len = strlen(payload);

    if (emcute_pub(&pub) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    // Set up the gateway address
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse gateway address");
        return 1;
    }

    // Start the EMCUTE thread
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Register the topic
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
