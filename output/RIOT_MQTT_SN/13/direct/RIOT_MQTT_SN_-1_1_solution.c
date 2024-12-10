#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define NUMOFSUBS (1U)
#define TOPIC_MAXLEN (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    printf("Received publication for topic '%s' [%i]: %s\n", topic->name, (int)len, in);
}

static int publish(char *topic_name, char *message)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    /* step 1: get topic id */
    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, message, strlen(message), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", topic_name);
        return 1;
    }

    printf("Published %s on topic %s\n", message, topic_name);

    return 0;
}

static void *mqtt_sn_publisher(void *arg)
{
    char *topic = "device/state";
    char *message = "work";
    (void)arg;

    /* wait for the network to be configured */
    xtimer_sleep(3);

    while (1) {
        publish(topic, message);
        xtimer_sleep(5);

        /* Toggle message for demonstration */
        if (strcmp(message, "work") == 0) {
            message = "not work";
        } else {
            message = "work";
        }
    }

    return NULL;
}

int main(void)
{
    puts("MQTT-SN publisher application\n");

    /* initialize emcute */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    /* the main thread needs a msg queue to be able to run `ping6` */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* network connect */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = 1883 };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1") == NULL) {
        puts("error: unable to parse IPv6 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, "device", NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    /* start publisher thread */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_sn_publisher, NULL, "mqtt_pub");

    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}