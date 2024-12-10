#include "ztimer.h"
#include "thread.h"
#include "string.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "msg.h"
#include "xtimer.h"
#include <stdio.h>

#define EMCUTE_ID           "device_id"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define STATE_TOPIC_NAME    "device/state"
#define MQTT_INTERVAL      (5U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    printf("[MQTT-SN Task] Initialization\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));
    
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT-SN Task] Connection failed");
        return 1;
    }

    topic.name = STATE_TOPIC_NAME;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[MQTT-SN Task] Topic registration failed");
        return 1;
    }

    while (1) {
        const char *state = "work";

        if (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("[MQTT-SN Task] Publish failed");
        }

        xtimer_sleep(MQTT_INTERVAL);
        state = (strcmp(state, "work") == 0) ? "not work" : "work";
    }

    return 0;
}