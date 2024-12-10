#include <string.h>
#include <stdio.h>
#include <msg.h>
#include <emcute.h>
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "riot-esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_TOPIC          "state"
#define MQTT_DATA_WORK      "work"
#define MQTT_DATA_NOT_WORK  "not work"
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         1883
#define PUBLISH_INTERVAL    (5U)

static msg_t emcute_queue[8];
static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic[TOPIC_MAXLEN];
static kernel_pid_t pid;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("### received publication for topic '%s' [%i] ###\n", topic->name, (int)topic->id);
    printf("data: %.*s\n", (int)len, in);
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(BROKER_PORT, EMCUTE_ID);
    return NULL; /* should never be reached */
}

static int connect_mqttsn_broker(void)
{
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }
    printf("Successfully connected to MQTT-SN broker on %s\n", BROKER_IP);
    return 0;
}

static void publish_state(void)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to obtain topic ID\n");
        return;
    }
    while (1) {
        char *data = MQTT_DATA_WORK;  // You can toggle this based on some condition
        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        } else {
            printf("Published %s to topic '%s'\n", data, MQTT_TOPIC);
        }
        xtimer_sleep(PUBLISH_INTERVAL);
    }
}

int main(void)
{
    msg_init_queue(emcute_queue, ARRAY_SIZE(emcute_queue));
    pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                        emcute_thread, NULL, "emcute");
    if (connect_mqttsn_broker() != 0) {
        return 1;
    }
    publish_state();
    return 0;
}