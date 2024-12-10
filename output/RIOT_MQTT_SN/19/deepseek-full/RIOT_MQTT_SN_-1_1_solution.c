
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

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;
static char payload[20];

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
    memset(&sub, 0, sizeof(sub));

    if (emcute_con(&sub, EMCUTE_ID, false, &sock) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&sub, &topic, TOPIC_NAME) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        // Simulate state change
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        snprintf(payload, sizeof(payload), "%s", state);

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
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    // Set gateway IP address
    ipv4_addr_t gw_addr = IPV4_ADDR(47, 102, 103, 1);
    memcpy(&gw.addr.ipv4, &gw_addr, sizeof(gw_addr));

    // Start the EMCUTE thread
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Run shell for debugging purposes
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
