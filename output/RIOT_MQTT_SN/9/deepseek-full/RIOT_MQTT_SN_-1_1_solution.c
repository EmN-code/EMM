
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

static char mqtt_sn_broker_addr_str[] = "47.102.103.1";
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char payload[16];
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;  // Toggle between "work" and "not work"

    emcute_pub(&topic, payload, strlen(payload), EMCUTE_QOS_0);
}

static void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (sock_udp_create(&local, NULL, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    if (emcute_con(&sub, EMCUTE_ID, false, &local) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example");

    if (inet_pton(AF_INET, mqtt_sn_broker_addr_str, &gw.addr.ipv4) <= 0) {
        puts("Error parsing MQTT-SN broker address");
        return 1;
    }

    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
