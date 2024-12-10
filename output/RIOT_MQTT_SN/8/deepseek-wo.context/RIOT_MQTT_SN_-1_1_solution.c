
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
#define EMCUTE_ID         "riot_mqtt_client"
#define EMCUTE_TOPIC      "state"
#define EMCUTE_MSG_WORK   "work"
#define EMCUTE_MSG_NOT_WORK "not work"
#define PUBLISH_INTERVAL  (5 * 1000000U)  /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = EMCUTE_PORT };
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT, .addr = { .ipv4_u32 = 0 } };

    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, "47.102.103.1") == NULL) {
        puts("Error: unable to parse IPv4 address");
        return NULL;
    }

    if (sock_udp_create(&sub.sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_topic_t topic;
    topic.name = EMCUTE_TOPIC;
    topic.id = 0;  /* topic ID will be assigned by the broker */

    if (emcute_sub(&sub, &topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return NULL;
    }

    while (1) {
        /* Publish state every 5 seconds */
        const char *msg = (random_uint32() % 2) ? EMCUTE_MSG_WORK : EMCUTE_MSG_NOT_WORK;
        emcute_pub(&sub, &topic, msg, strlen(msg), EMCUTE_QOS_0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN Client Example");

    /* Initialize the network interface */
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    /* Start the EMCUTE thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    /* Run the shell to allow manual interaction */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
