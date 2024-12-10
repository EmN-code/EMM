#include <timex.h>
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include "xtimer.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC          "status"
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_BROKER_PORT    (1883)
#define PUBLISH_INTERVAL    (5U)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t status_topic;

static char* status_messages[] = {"work", "not work"};

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PRIO, emcute_stack, sizeof(emcute_stack));
    return NULL;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    if (inet_pton(AF_INET6, MQTT_BROKER_IP, &gw.addr) != 1) {
        puts("Error: unable to parse gateway address");
        return 1;
    }
    if (emcute_con(&gw, true, nullptr, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }
    return 0;
}

static int publish_status(int idx) {
    emcute_msg_t msg;
    msg.data = status_messages[idx];
    msg.len = strlen(msg.data);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_QOS_0;
    emcute_pub(&status_topic, &msg);
    return 0;
}

int main(void) {
    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    status_topic.name = MQTT_TOPIC;
    
    if (connect_broker() != 0) {
        return 1;
    }

    unsigned int current_status = 0;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        publish_status(current_status);
        current_status = (current_status + 1) % 2; // Toggle between 0 and 1
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL * US_PER_SEC);
    }

    return 0;
}