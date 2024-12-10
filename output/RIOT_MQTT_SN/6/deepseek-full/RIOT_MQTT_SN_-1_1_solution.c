
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/af.h"
#include "net/conn/udp.h"
#include "net/netif.h"
#include "shell.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ADDR       "47.102.103.1"
#define EMCUTE_TOPIC      "device/state"
#define EMCUTE_QOS        0
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, EMCUTE_ADDR) == NULL) {
        puts("Error: unable to parse IPv4 address");
        return NULL;
    }

    if (sock_udp_create(&sock_udp, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));
    sub.cb = NULL;
    sub.topic = &topic;

    if (emcute_con(&sub, EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        char payload[20];
        strcpy(payload, "work");  // or "not work" based on your logic

        if (emcute_pub(&sub, payload, strlen(payload), EMCUTE_QOS) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            printf("Published: %s\n", payload);
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN Client starting");

    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Set up the MQTT-SN topic */
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the broker

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  emcute_thread, NULL, "emcute");

    /* Run the shell to allow manual interaction */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
