#include "net/gcoap.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "thread.h"

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define MQTT_SN_ADDRESS     "47.102.103.1"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static int mqtt_publish_state(void) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    char *topic_name = "state";
    char *message = "work or not work";

    t.name = topic_name;

    if (emcute_reg(&t) != EMCUTE_OK) {
        return 1;
    }

    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        return 1;
    }

    return 0;
}

void *emcute_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_SN_ADDRESS) == NULL) {
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return NULL;
    }

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    return 0;
}