
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
static emcute_topic_t t;
static char topic_str[64];

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
    t.name = topic_str;
    t.id = 0;

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        emcute_req_t req;
        req.cb = NULL;
        req.topic = &t;
        req.data = "work";
        req.data_len = strlen("work");

        if (emcute_pub(&req) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN example");

    /* set a name for the node */
    emcute_set_client_id(EMCUTE_ID);

    /* parse the IP address of the broker */
    if (ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error parsing IP address");
        return 1;
    }

    /* start the emcute thread */
    thread_create(NULL, 2048, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* initialize the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
