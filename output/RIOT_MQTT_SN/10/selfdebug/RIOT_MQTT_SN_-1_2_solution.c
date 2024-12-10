#include <xtimer.h>
#include <string.h>
#include "net/emcute.h"
#include "msg.h"

#define NUMOF_MSGS     (8U)
#define EMCUTE_PORT    (1883U)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t emcute_queue[NUMOF_MSGS];
static emcute_topic_t topic;
static const char *state = "work";

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Callback for handling incoming publishs
}

int main(void) {
    /* initialize our emcute thread */
    msg_init_queue(emcute_queue, NUMOF_MSGS);
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_run, NULL, "emcute_thread");

    /* define gateway address and port */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, "47.102.103.1");

    /* connect to MQTT-SN gateway */
    if (emcute_con(&gw, true, "will/topic", "bye", strlen("bye"), 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* set the topic we publish to */
    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        /* publish to topic */
        printf("Publishing payload: %s\n", state);
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        }
        /* change the state for every five seconds */
        state = (state == "work") ? "not work" : "work";
        xtimer_sleep(5);
    }

    /* disconnect from the gateway */
    emcute_discon();
    return 0;
}