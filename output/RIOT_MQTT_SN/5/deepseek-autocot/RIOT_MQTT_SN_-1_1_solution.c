
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
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;
static char emcute_buf[128];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP sock");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));
    sub.cb = NULL;
    sub.topic = &topic;

    while (1) {
        /* Connect to the MQTT-SN broker */
        if (emcute_con(&sub, true, EMCUTE_ID, NULL, 0, 0, 0) != EMCUTE_OK) {
            puts("Error on connection");
            xtimer_sleep(1);
            continue;
        }

        /* Publish state every 5 seconds */
        while (1) {
            const char *state = (random_uint32() % 2) ? "work" : "not work";
            printf("Publishing state: %s\n", state);
            snprintf(emcute_buf, sizeof(emcute_buf), "%s", state);

            if (emcute_pub(&sub, &topic, EMCUTE_QOS_0, emcute_buf, strlen(emcute_buf), 0) != EMCUTE_OK) {
                puts("Error on publish");
                break;
            }

            xtimer_usleep(PUBLISH_INTERVAL);
        }

        /* Disconnect from the MQTT-SN broker */
        emcute_discon(&sub);
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

    /* Set the gateway address */
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    gw.addr.ipv4 = gw_addr;

    /* Initialize the topic */
    topic.name = TOPIC_NAME;
    topic.id = 0;

    /* Start the EMCUTE thread */
    thread_create(emcute_buf, sizeof(emcute_buf), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Run the shell to allow manual interaction */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
