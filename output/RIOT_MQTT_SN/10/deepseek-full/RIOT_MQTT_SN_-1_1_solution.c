
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

static char mqtt_sn_broker_addr[IPV6_ADDR_MAX_STR_LEN];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    char payload[16];
    snprintf(payload, sizeof(payload), "%s", states[state_index]);
    state_index = (state_index + 1) % 2;  // Toggle between "work" and "not work"

    emcute_pub_t pub;
    pub.topic = &topic;
    pub.qos = EMCUTE_QOS_0;
    pub.retain = false;
    pub.data = payload;
    pub.data_len = strlen(payload);

    if (emcute_pub(&pub) != EMCUTE_OK) {
        puts("Error: unable to publish message");
    } else {
        printf("Published: %s\n", payload);
    }
}

static void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    if (sock_udp_create(&local, NULL, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.topic = &topic;
    sub.cb = NULL;
    sub.qos = EMCUTE_QOS_0;

    if (emcute_con(&sub, true, EMCUTE_ID) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        xtimer_usleep(PUBLISH_INTERVAL);
        publish_state();
    }

    return NULL;
}

int main(void) {
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("Error: no network interface found");
        return 1;
    }

    // Set the MQTT-SN broker address
    ipv6_addr_t broker_addr;
    if (ipv6_addr_from_str(&broker_addr, "47.102.103.1") == NULL) {
        puts("Error: invalid MQTT-SN broker address");
        return 1;
    }
    gw.addr.ipv6 = broker_addr;

    // Initialize the topic
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the broker

    // Start the EMCUTE thread
    thread_create(NULL, 1024, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Run the shell to allow manual interaction
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
