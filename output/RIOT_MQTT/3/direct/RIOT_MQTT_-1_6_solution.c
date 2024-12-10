#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <periph/pm.h>

#define EMCUTE_ID          "gertrud"
#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_TOPIC         "device/state"
#define INTERVAL           (5U)

/* Add define for broker address */
static const char *mqtt_broker_address = MQTT_BROKER_IP;

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, mqtt_broker_address);
    return NULL; /* should never be reached */
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* wait to ensure emcute thread has started */
    xtimer_sleep(1);
    
    /* define emcute endpoint */
    emcute_ep_t ep = {
        .port = EMCUTE_PORT,
        .host = mqtt_broker_address
    };

    /* connect to broker */
    if (emcute_con(&ep, true, EMCUTE_ID, NULL, NULL, 0) == -1) {
        puts("error: unable to connect to MQTT broker
");
        return 1;
    }

    while (1) {
        const char *state = "work"; // Here you can change state to "not work" as needed
        if (emcute_pub(MQTT_TOPIC, state, strlen(state), EMCUTE_QOS_0) == -1) {
            puts("error: unable to publish data
");
        } else {
            printf("Published: %s\n", state);
        }
        xtimer_sleep(INTERVAL);
    }

    return 0;
}