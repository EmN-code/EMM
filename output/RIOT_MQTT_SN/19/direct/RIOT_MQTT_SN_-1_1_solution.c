#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include <string.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_TOPIC      "state"
#define MQTT_BROKER_IP  "47.102.103.1"
#define MSG_INTERVAL    (5U) // Message interval in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher(void *arg) {
    (void)arg;
    emcute_connect(NULL, MQTT_BROKER_IP, EMCUTE_PORT, NULL, NULL, 0);

    while (1) {
        const char *state = "work";
        emcute_pub(MQTT_TOPIC, state, strlen(state), EMCUTE_QOS_0,
                   EMCUTE_MSG_RETAINED);
        xtimer_sleep(MSG_INTERVAL);

        state = "not work";
        emcute_pub(MQTT_TOPIC, state, strlen(state), EMCUTE_QOS_0,
                   EMCUTE_MSG_RETAINED);
        xtimer_sleep(MSG_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("MQTT-SN publisher using emcute started");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    /* Main thread will do nothing and let the publisher thread handle the work */
    thread_sleep();

    return 0;
}
