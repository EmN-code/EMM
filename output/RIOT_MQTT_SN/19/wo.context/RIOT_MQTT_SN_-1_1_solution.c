#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <net/emcute.h>
#include <periph/gpio.h>

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32-node"
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME      "esp32/state"
#define BROKER_PORT     "1883"
#define BROKER_ADDR     "47.102.103.1"
#define LOOP_DURATION   (5U * 1000U * 1000U) // 5 seconds in microseconds

static emcute_topic_t _mqtt_topic;
static char _mqtt_topic_name[] = TOPIC_NAME;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

static int _publish(const char *state) {
    int res;
    emcute_msg_t msg;

    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_MSG_FLAG_RETAIN;

    res = emcute_pub(&_mqtt_topic, &msg);

    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data (%d)\n", res);
        return res;
    }

    printf("Published %s on topic %s\n", state, _mqtt_topic_name);
    return EMCUTE_OK;
}

int main(void) {
    /* initialize emcute */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  _emcute_thread, NULL, "emcute");

    /* connect to broker */
    sock_udp_ep_t gw = { .family = AF_INET, .port = atoi(BROKER_PORT) };
    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, BROKER_ADDR);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    /* set topic */
    _mqtt_topic.name = _mqtt_topic_name;

    /* start publishing loop */
    while (1) {
        _publish("work");
        ztimer_sleep(ZTIMER_USEC, LOOP_DURATION);
        _publish("not work");
        ztimer_sleep(ZTIMER_USEC, LOOP_DURATION);
    }

    return 0;
}