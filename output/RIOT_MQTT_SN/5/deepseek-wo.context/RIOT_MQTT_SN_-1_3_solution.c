
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work"

static char stack[THREAD_STACKSIZE_DEFAULT];
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };
static emcute_topic_t topic;

void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    if (sock_udp_ep_from_str(&gw, EMCUTE_ADDR) < 0) {
        puts("Error parsing gateway address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error connecting to MQTT-SN gateway");
        return NULL;
    }

    puts("Connected to MQTT-SN gateway");

    topic.name = TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error registering topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, MSG, strlen(MSG), 0) != EMCUTE_OK) {
            puts("Error publishing message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
