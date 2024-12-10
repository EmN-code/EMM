#include "string.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "thread.h"
#include "timex.h"

#define EMCUTE_ID           "board"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         1883
#define MQTT_BROKER_IP      "47.102.103.1"
#define TOPIC_NAME          "device/state"
#define PUB_INTERVAL        (5U) /* in seconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, EMCUTE_ID, EMCUTE_PRIO);
    return NULL;
}

static int mqtt_connect(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("[MQTT] Error: unable to parse gateway address");
        return -1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT] Error: unable to connect to gateway");
        return -1;
    }
    return 0;
}

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, state, strlen(state), flags) != EMCUTE_OK) {
        puts("[MQTT] Error: unable to publish data");
        return -1;
    }
    return 0;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    if (mqtt_connect() != 0) {
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[MQTT] Error: unable to register topic");
        return 1;
    }

    while (1) {
        const char *state = "work or not work";
        if (publish_state(state) != 0) {
            puts("[MQTT] Error: unable to publish state");
        }
        ztimer_sleep(ZTIMER_MSEC, PUB_INTERVAL * MS_PER_SEC);
    }

    return 0;
}
