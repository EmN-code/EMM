
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
#define EMCUTE_ADDR       "47.102.103.1"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;
static char topic_str[] = TOPIC_NAME;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.cb = NULL;
    sub.topic = &topic;

    if (emcute_con(&sub, EMCUTE_ADDR, EMCUTE_PORT) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        char payload[20];
        strcpy(payload, "work");  // or "not work" based on your logic

        if (emcute_pub(&sub, &topic, payload, strlen(payload), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", payload);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    /* Set up the MQTT-SN topic */
    topic.name = topic_str;
    topic.id = 0;  // Topic ID will be assigned by the broker

    /* Start the EMCUTE thread */
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* Main loop */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
