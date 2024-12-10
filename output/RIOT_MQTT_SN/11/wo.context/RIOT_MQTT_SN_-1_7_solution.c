#include <string.h>
#include <stdio.h>
#include "net/emcute.h"
#include "msg.h"
#include "xtimer.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUB_INTERVAL (5U * US_PER_SEC)

static emcute_topic_t _state_topic;
static char _mqtt_topic_name[] = "state";

static void *pub_thread(void *arg) {
    (void)arg;
    while (1) {
        const char *state = (xtimer_now_usec() % 2) ? "work" : "not work";
        emcute_pub(&_state_topic, state, strlen(state), EMCUTE_QOS_0);
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_t(0), 8);

    static sock_udp_ep_t gw = {.family = AF_INET6,
                               .port = MQTT_BROKER_PORT};
    inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr.ipv6);

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    _state_topic.name = _mqtt_topic_name;
    emcute_reg(&_state_topic);

    char pub_thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(pub_thread_stack, sizeof(pub_thread_stack), THREAD_PRIORITY_MAIN - 1, 0, pub_thread, NULL, "pub thread");

    return 0;  // Start the thread and exit main
}