#include <string.h>   // Provides string manipulation functions
#include "xtimer.h"  // Provides access to the xtimer for delay functions
#include "thread.h"  // Provides threading support functionalities
#include "net/emcute.h"  // emCute is the MQTT-SN client library

#define STATE_MSG_WORK "work"
#define STATE_MSG_NOT_WORK "not work"
#define MQTT_SN_BROKER_ADDRESS "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define STATE_TOPIC "state"
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 1)
#define PAYLOAD_SIZE (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];  // stack for the emcute thread
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_SN_BROKER_PORT, "esp32_client");
    return NULL;
}

static int publish_state(int work_state) {
    const char *state;
    if (work_state) {
        state = STATE_MSG_WORK;
    } else {
        state = STATE_MSG_NOT_WORK;
    }
    return emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    // initialize the stack and start the emcute thread
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute_thread");

    // define the broker endpoint
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = MQTT_SN_BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_SN_BROKER_ADDRESS);

    // attempt to connect to MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to MQTT-SN broker.");
        return 1;
    }
    puts("Successfully connected to MQTT-SN broker");

    // register a topic
    topic.name = STATE_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID.");
        return 1;
    }

    int work_state = 0;
    while (1) {
        publish_state(work_state);
        work_state = !work_state;  // toggle between work and not work
        xtimer_sleep(5);
    }

    return 0;
}