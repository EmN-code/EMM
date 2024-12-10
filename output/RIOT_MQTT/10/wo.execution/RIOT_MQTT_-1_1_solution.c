#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "isrpipe.h"
#include "net/emcute.h"
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gnrc.h"
#include "random.h"
#include "string.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_PERIOD_SEC 5

static char stack[THREAD_STACKSIZE_MAIN];

static void *mqtt_thread(void *arg) {
    (void)arg;  // we don't use this parameter
    sock_udp_t sock;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, MQTT_BROKER_IP);
    static const char *state_msg[] = { "work", "not work" };
    int state_index = 0;  // initial state, starts with "work"

    if (sock_udp_create(&sock, NULL, NULL, 0) < 0) {
        puts("Error: unable to create UDP sock");
        return NULL;
    }

    while (1) {
        const char *msg = state_msg[state_index];
        if (sock_udp_send(&sock, msg, strlen(msg), &remote) < 0) {
            printf("Error: unable to send to %s\n", MQTT_BROKER_IP);
        } else {
            printf("State published: %s\n", msg);
        }

        state_index = (state_index + 1) % 2;  // Swap state between 0 and 1
        xtimer_sleep(PUBLISH_PERIOD_SEC);
    }

    return NULL;
}

int main(void) {
    puts("MQTT state publisher example started");
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");
    return 0;
}
