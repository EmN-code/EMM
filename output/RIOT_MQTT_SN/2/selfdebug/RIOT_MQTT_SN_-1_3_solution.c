#include "net/sock/udp.h"
#include "net/emcute.h"
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_INTERVAL    5
#define MQTT_STATE_TOPIC "state"
#define MQTT_STATE_MESSAGE "work"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_sub_t subscriptions[1];
static char *topicsubs[1];

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, "my_client_id");
    return NULL;
}

int main(void) {
    /* initialize the message queue */
    msg_init_queue(queue, 8);

    /* setup the EMCUTE thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS);

    /* try to connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    emcute_topic_t topic;
    topic.name = MQTT_STATE_TOPIC;
    topic.id = 0;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain a topic ID\n");
        return 1;
    }

    while (1) {
        if (emcute_pub(&topic, MQTT_STATE_MESSAGE, sizeof(MQTT_STATE_MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data\n");
        } else {
            puts("Published data successfully\n");
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}