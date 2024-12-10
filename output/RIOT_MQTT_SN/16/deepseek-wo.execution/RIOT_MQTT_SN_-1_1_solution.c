
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

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_sub_t sub;
    sock_udp_t sock;

    /* parse address */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse broker address");
        return NULL;
    }

    /* initialize EMCUTE client */
    emcute_init(EMCUTE_ID);

    /* connect to MQTT-SN broker */
    if (sock_udp_create(&sock, NULL, &gw, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    /* register topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    /* main loop */
    while (1) {
        /* publish state */
        const char *state = (random_uint32() % 2) ? "work" : "not work";
        if (emcute_pub(&topic, state, strlen(state)) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }

        /* wait for 5 seconds */
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    /* start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* initialize the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
