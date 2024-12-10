#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "net/gcoap.h"
#include "arpa/inet.h"

#define EMCUTE_ID           ("gertrud")
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define SERVER_IP           "47.102.103.1"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

/* MQTT-SN topics */
static emcute_sub_t subscriptions[NUMOFSUBS];
static size_t num_subscriptions = 0;

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received publication of topic '%s' [%i]: %.*s\n",
           topic->name, topic->id, (int)len, (char *)data);
}

static int publish(const char *topic_name, const char *data)
{
    /* Step 3: Register the topic */
    unsigned flags = EMCUTE_QOS_0;
    topic.name = topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* Step 4: Publish data */
    if (emcute_pub(&topic, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("Published %s on topic %s\n", data, topic.name);
    return 0;
}

static void *mqtt_thread(void *arg)
{
    char *state_message = (char *)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) == 0) {
        puts("error: invalid address\n");
        return NULL;
    }

    /* Step 2: Connect to the broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return NULL;
    }
    puts("Connected\n");

    /* Publish the message every 5 seconds */
    while (1) {
        publish("device/state", state_message);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Step 1: Run the emCute stack */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    const char *state_message = "work";
    if (thread_create(stack, sizeof(stack),
                      EMCUTE_PRIO, 0, mqtt_thread, (void *)state_message,
                      "mqtt") <= KERNEL_PID_UNDEF) {
        puts("error: unable to create thread\n");
        return 1;
    }

    return 0;
}