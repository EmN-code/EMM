#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_ID          "esp32"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)

#define BROKER_IP          "47.102.103.1"

static emcute_topic_t _state;
static char _state_message[] = "work";

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(_emcute_queue, sizeof(_emcute_queue) / sizeof(_emcute_queue[0]));
    
    emcute_sock_t sock;
    sock_udp_ep_t broker = { .family = AF_INET, .port = EMCUTE_PORT };

    if (ipv4_addr_from_str((ipv4_addr_t *)&broker.addr.ipv4, BROKER_IP) == NULL) {
        printf("Error: unable to parse broker address\n");
        return NULL;
    }

    int res = emcute_con((sock_udp_ep_t *)&broker, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);
        return NULL;
    }

    printf("Connected to [%s]:%d\n", BROKER_IP, EMCUTE_PORT);

    while (1) {
        xtimer_sleep(5);
        int res = emcute_pub(&_state, _state_message, strlen(_state_message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        } else {
            printf("Published: %s\n", _state_message);
        }
    }

    return NULL;
}

int main(void) {
    static char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    return 0;
}
