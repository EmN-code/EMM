
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

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    sub.topic = &topic;
    sub.cb = NULL;
    sub.qos = EMCUTE_QOS;

    if (emcute_con(&sub, EMCUTE_ADDR, EMCUTE_PORT, EMCUTE_QOS) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    while (1) {
        static const char *states[] = {"work", "not work"};
        static int state_index = 0;

        char payload[32];
        snprintf(payload, sizeof(payload), "{\"state\": \"%s\"}", states[state_index]);

        if (emcute_pub(&sub, &topic, payload, strlen(payload), EMCUTE_QOS) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", payload);
        }

        state_index = (state_index + 1) % 2;
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
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  /* Topic ID will be assigned by the broker */

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* Run the shell to allow manual interaction */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
