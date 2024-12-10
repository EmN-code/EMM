
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

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t t;
static char emcute_buf[128];

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
    t.name = TOPIC_NAME;
    t.id = 0;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        // Publish state every 5 seconds
        const char *state = (random_uint32() % 2 == 0) ? "work" : "not work";
        printf("Publishing state: %s\n", state);
        if (emcute_pub(&t, state, strlen(state)) != EMCUTE_OK) {
            puts("Error publishing message");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    // Set the gateway address
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Start the EMCUTE thread
    thread_create(emcute_buf, sizeof(emcute_buf), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    // Run the shell to allow manual interaction
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
