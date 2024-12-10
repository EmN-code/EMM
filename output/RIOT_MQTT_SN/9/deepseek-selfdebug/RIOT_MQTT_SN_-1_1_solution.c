
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
#define TOPIC             "state"
#define STATE_MESSAGE     "work or not work"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = EMCUTE_PORT;
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, "47.102.103.1") == NULL) {
        puts("Error: unable to parse IP address");
        return NULL;
    }

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error: unable to create UDP socket");
        return NULL;
    }

    emcute_sub_t sub;
    memset(&sub, 0, sizeof(sub));

    if (emcute_con(&sub, EMCUTE_ID, true, THREAD_PRIORITY_MAIN - 1) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    if (emcute_reg(&sub, &topic, TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&sub, &topic, STATE_MESSAGE, strlen(STATE_MESSAGE), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published successfully");
        }

        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    puts("RIOT MQTT-SN Client Example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
