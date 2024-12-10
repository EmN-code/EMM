#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/inet_sock.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

/* add address and port of the MQTT-SN broker here */
static const sock_udp_ep_t server = { .family = AF_INET, .ipv4 = { .uint32 = IPV4_ADDR(47,102,103,1) },.port = 1883 };

static void *emcute_thread(void *arg)
{
    (void) arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, "esp32");
    return NULL;    /* should never be reached */
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

int main(void)
{
    /* the main thread needs a msg queue to be able to run `emcute_run()` */
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* start emcute thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    /* connect to MQTT-SN broker */
    if (emcute_con((sock_udp_ep_t *) &server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }
    puts("Successfully connected to gateway\n");

    char *topic = "esp32/state";
    char *message = "work";

    /* publish the state every 5 seconds */
    while (1) {
        publish(topic, message);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    /* disconnect from the broker */
    emcute_discon();

    return 0;
}