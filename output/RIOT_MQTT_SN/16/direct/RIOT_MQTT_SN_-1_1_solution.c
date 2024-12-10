#include <xtimer.h>
#include <stdlib.h>
#include <string.h>
#include <emcute.h>
#include <net/emcute.h>
#include <net/gnrc.h>

#define EMCUTE_PORT     (1883U)
#define EMCUTE_ID       ("mqtt-sample")
#define EMCUTE_PRIO     (THREAD_PRIORITY_MAIN - 1)
#define MQTT_TOPIC      ("state")
#define MQTT_STATE_WORK ("work")
#define MQTT_STATE_NOWORK ("not work")

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sock_t sock;
static msg_t queue[8];

typedef struct mqtt_state {
    const char *topic;
    const char *message;
} mqtt_state_t;

mqtt_state_t states[] = {
    { MQTT_TOPIC, MQTT_STATE_WORK },
    { MQTT_TOPIC, MQTT_STATE_NOWORK }
};

/**
 * @brief   The callback function to be called when a message is published to the topic.
 */

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    (void)data;
    (void)len;
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, stack, THREAD_STACKSIZE_MAIN, EMCUTE_PRIO);
    return NULL;
}

static int publish(const char *topic, const char *data, int qos)
{
    unsigned flags = EMCUTE_QOS_0;
    emcute_topic_t t;
    t.name = topic;

    if (qos == 1) {
        flags = EMCUTE_QOS_1;
    }
    else if (qos == 2) {
        flags = EMCUTE_QOS_2;
    }

    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        puts("error: unable to publish data\n");
        return 1;
    }

    puts("Published data\n");
    return 0;
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize our subscription buffer */
    sock.cb = on_pub;

    /* start the emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to broker */
    sock.ipv6_ep.flags = ENET_FLAG_ENONE;
    sock.ipv6_ep.port = EMCUTE_PORT;
    ipv6_addr_set_null(&sock.ipv6_ep.addr);

    if (emcute_con(&sock) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    puts("Successfully connected to broker\n");

    /* alternating between work and not work states */
    mqtt_state_t current_state;
    while (1) {
        for (int i = 0; i < sizeof(states)/sizeof(states[0]); ++i) {
            current_state = states[i];
            publish(current_state.topic, current_state.message, 0);
            xtimer_sleep(5);
        }
    }

    return 0;
}