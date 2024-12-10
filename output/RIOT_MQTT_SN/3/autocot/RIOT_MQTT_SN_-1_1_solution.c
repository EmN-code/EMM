#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_ID           "my_device"
#define EMCUTE_PORT         1883
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_STACK_SIZE   (THREAD_STACKSIZE_MAIN)

static char emcute_stack[EMCUTE_STACK_SIZE];
static msg_t emcute_queue[8];

#define MQTT_TOPIC "state"
#define MQTT_DATA_WORK "work"
#define MQTT_DATA_NOTWORK "not work"

static emcute_topic_t _t;
static char _state[64] = "work";

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, emcute_queue, ARRAY_SIZE(emcute_queue), EMCUTE_ID);
    return NULL;
}

static int publish_state(const char *state) {
    _t.name = MQTT_TOPIC;

    if (emcute_reg(&_t) != EMCUTE_OK) {
        puts("[Error] unable to register topic");
        return 1;
    }

    emcute_pub(&_t, state, strlen(state), EMCUTE_QOS_0);

    return 0;
}

int main(void) {
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  _emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, NULL, 0) != EMCUTE_OK) {
        puts("[Error] unable to connect to MQTT-SN broker");
        return 1;
    }

    while (1) {
        publish_state(_state);
        _state[0] = (_state[0] == 'w') ? 'n' : 'w'; // toggle state
        xtimer_sleep(5);
    }

    return 0;
}