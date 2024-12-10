#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "netinet/in.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "dht_params.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT     (1883U)
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_ID       "myRIOT"
#define SERVER_IP       "47.102.103.1"
#define PUB_INTERVAL    (5U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t my_topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse IPv4 address");
        return 1;
    }

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Could not connect to gateway: %d\n", res);
        return 1;
    }

    my_topic.name = "state";
    res = emcute_reg(&my_topic);
    if (res != EMCUTE_OK) {
        printf("Could not register topic: %d\n", res);
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    while (1) {
        const char *state = ((rand() % 2) == 0) ? "work" : "not work";
        printf("Publishing state: %s\n", state);

        if (emcute_pub(&my_topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Failed to publish data");
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}