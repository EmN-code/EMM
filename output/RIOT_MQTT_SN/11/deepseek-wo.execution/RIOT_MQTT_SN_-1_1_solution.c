
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
#define EMCUTE_ID         "riot_esp32"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG_WORK   "work"
#define EMCUTE_MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT, .addr = { .ipv4 = { 47, 102, 103, 1 } } };

    if (sock_udp_create(&sub.sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    emcute_topic_t topic;
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  // Topic ID will be assigned by the gateway

    while (1) {
        // Publish state message
        const char *state = (random_uint32() % 2) ? EMCUTE_MSG_WORK : EMCUTE_MSG_NOT_WORK;
        if (emcute_pub(&sub, &topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            printf("Published: %s\n", state);
        }

        // Wait for 5 seconds
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example");

    // Initialize the network interface
    netif_t *netif = netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    // Start the EMCUTE thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    // Run the shell to allow manual interaction
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
