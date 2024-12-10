#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT         1883
#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC          "state"

static emcute_sock_t sock;
static char stack[THREAD_STACKSIZE_DEFAULT];
static void *emcute_thread(void *arg) {
    (void)arg;   /* unused */
    emcute_run(&sock, EMCUTE_PORT, stack, sizeof(stack), EMCUTE_PRIO);
    return NULL;
}

int main(void){
    /* Initialize emcute */
    memset(&sock, 0, sizeof(sock));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    /* Connect to the broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    if (emcute_con(&sock, &gw, true, EMCUTE_ID, NULL, NULL, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", gw.addr.ipv6, EMCUTE_PORT);
        return 1;
    }

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = MQTT_TOPIC;

    while (1) {
        /* Publish to the topic */
        const char *state = "work";
        if (emcute_pub(&sock, &t, state, strlen(state), flags) != EMCUTE_OK) {
            printf("error: unable to publish to topic '%s'\n", t.name);
        }
        xtimer_sleep(5);
    }

    return 0;
}