
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/netif.h"
#include "net/sock/udp.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static emcute_topic_t topic;
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };

    if (sock_udp_create(&local, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int connect_to_broker(void)
{
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error parsing broker address");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return -1;
    }

    puts("Connected to MQTT-SN broker");
    return 0;
}

static int register_topic(void)
{
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return -1;
    }

    printf("Registered topic: %s with ID %u\n", topic.name, topic.id);
    return 0;
}

static void publish_state(void)
{
    const char *state = "work";
    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error publishing state");
    } else {
        puts("State published");
    }
}

int main(void)
{
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    if (connect_to_broker() != 0) {
        return 1;
    }

    if (register_topic() != 0) {
        return 1;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
