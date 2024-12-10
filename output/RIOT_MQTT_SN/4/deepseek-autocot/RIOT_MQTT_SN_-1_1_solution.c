
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
#include "msg.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static sock_udp_t sock;
static emcute_topic_t topic;
static char payload[32];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4) <= 0) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    if (sock_udp_create(&sock, NULL, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));
    sub.cb = NULL;
    sub.topic = &topic;

    if (emcute_con(&sub, EMCUTE_ID, true, &sock) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        // Publish "work" or "not work" every 5 seconds
        strcpy(payload, "work");
        emcute_pub(&sub, &topic, payload, strlen(payload), 0);
        xtimer_usleep(PUBLISH_INTERVAL);

        strcpy(payload, "not work");
        emcute_pub(&sub, &topic, payload, strlen(payload), 0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    // Initialize the MQTT-SN topic
    topic.name = TOPIC_NAME;
    topic.id = 0;  // Topic ID will be assigned by the broker

    // Start the EMCUTE thread
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Main loop (optional, can be used for additional tasks)
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
