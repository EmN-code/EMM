
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/udp.h"
#include "net/gnrc.h"
#include "shell.h"

#define EMCUTE_PORT "1883"
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define QOS 0

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t local = { .family = AF_INET, .port = atoi(EMCUTE_PORT) };
    sock_udp_ep_t remote = { .family = AF_INET, .port = atoi(EMCUTE_PORT), .addr = EMCUTE_ADDR };

    if (sock_udp_create(&sub.sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return NULL;
    }

    while (1) {
        char buf[128];
        ssize_t res = sock_udp_recv(&sub.sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, NULL);
        if (res < 0) {
            puts("Error receiving data");
            continue;
        }
        buf[res] = '\0';
        printf("Received: %s\n", buf);
    }

    return NULL;
}

static void publish_state(void)
{
    static int state = 0;
    const char *states[] = { "work", "not work" };

    char msg[32];
    sprintf(msg, "%s", states[state]);
    state = !state;

    emcute_topic_t t;
    t.name = TOPIC;
    t.qos = QOS;

    if (emcute_pub(&t, msg, strlen(msg)) != EMCUTE_OK) {
        puts("Error publishing message");
    } else {
        printf("Published: %s\n", msg);
    }
}

int main(void)
{
    puts("MQTT-SN client starting");

    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return 0;
}
