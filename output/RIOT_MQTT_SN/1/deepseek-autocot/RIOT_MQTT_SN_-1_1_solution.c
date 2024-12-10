
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
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG_WORK   "work"
#define EMCUTE_MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL  (5 * 1000000U) /* 5 seconds in microseconds */

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t t;
static char emcute_buf[128];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = { .family = AF_INET, .port = 0 };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, &gw, 0) < 0) {
        puts("Error creating UDP sock");
        return NULL;
    }

    emcute_sub_t sub;
    t.name = EMCUTE_TOPIC;
    t.id = 0;

    while (1) {
        if (emcute_con(&sub, EMCUTE_ID, &sock, &gw) != EMCUTE_OK) {
            puts("Error connecting to MQTT-SN broker");
            xtimer_sleep(1);
            continue;
        }

        if (emcute_reg(&sub, &t) != EMCUTE_OK) {
            puts("Error registering topic");
            emcute_discon(&sub);
            xtimer_sleep(1);
            continue;
        }

        while (1) {
            /* Publish state every 5 seconds */
            snprintf(emcute_buf, sizeof(emcute_buf), "%s", EMCUTE_MSG_WORK);
            if (emcute_pub(&sub, &t, emcute_buf, strlen(emcute_buf)) != EMCUTE_OK) {
                puts("Error publishing message");
                break;
            }
            xtimer_usleep(PUBLISH_INTERVAL);

            snprintf(emcute_buf, sizeof(emcute_buf), "%s", EMCUTE_MSG_NOT_WORK);
            if (emcute_pub(&sub, &t, emcute_buf, strlen(emcute_buf)) != EMCUTE_OK) {
                puts("Error publishing message");
                break;
            }
            xtimer_usleep(PUBLISH_INTERVAL);
        }

        emcute_discon(&sub);
    }

    return NULL;
}

int main(void)
{
    /* Initialize network interface */
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    /* Set static IP address */
    ipv6_addr_t addr = IPV6_ADDR_UNSPECIFIED;
    netif_set_opt(netif, NETOPT_IPV6_ADDR, 0, &addr, sizeof(addr));

    /* Start the EMCUTE thread */
    thread_create(emcute_buf, sizeof(emcute_buf), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    /* Main loop */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
