#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdlib.h"
#include "stdout.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PORT         (1883U)
#define MQTT_BROKER_IP      "47.102.103.1"
#define MQTT_TOPIC          "state"
#define MQTT_STATE_WORK     "work"
#define MQTT_STATE_NOT_WORK "not work"
#define SLEEP_INTERVAL      (5U * ONESEC) // 5 seconds interval

// Definition of commonly used macros
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static char *_mqtt_topic = NULL;
static emcute_topic_t _monitor_topic;

static sock_udp_ep_t broker_ep;

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    
    // Initialize the sock_udp_ep_t for the broker
    broker_ep.family = AF_INET;
    broker_ep.port = EMCUTE_PORT;
    inet_pton(AF_INET, MQTT_BROKER_IP, &broker_ep.addr.ipv4);

    // Connect to the broker
    if (emcute_con(&broker_ep, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to [%s]:%u\n", MQTT_BROKER_IP, (unsigned)EMCUTE_PORT);
        return NULL;
    }

    // Register the topic
    _mqtt_topic = MQTT_TOPIC;
    _monitor_topic.name = _mqtt_topic;
    if (emcute_reg(&_monitor_topic) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return NULL;
    }

    // Main publishing loop
    while (1) {
        const char *state = MQTT_STATE_WORK;  // Placeholder for dynamic state check logic.
        size_t len = strlen(state);

        // Publish the state
        if (emcute_pub(&_monitor_topic, state, len, EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }

        // Sleep for a defined interval
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL / ONESEC);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");
    return 0;
}