#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_name[TOPIC_MAXLEN];

static const char *status_messages[] = {
    "work",
    "not work"
};

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;
    (void)topic;

    printf("### received publication for topic '%s' [%i] ###\n",
           topic->name, (int)topic->id);
    printf("%.*s\n", (int)len, in);
}

/* Setup the emcute client thread and connect to broker */
static int setup_connection(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    emcute_conf_t conf = { "mynode", topic_name, EMCUTE_PRIO, stack, sizeof(stack), queue, 8,
                           { subscriptions, NUMOFSUBS } };
    emcute_init(&conf, on_pub);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN gateway\n");
        return 1;
    }

    return 0;
}

int main(void)
{
    /* Initialize periphery and network */
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));

    /* Setup connection to the broker */
    if (setup_connection() != 0) {
        return 1;
    }

    emcute_topic_t topic;
    sprintf(topic_name, "device/state");
    topic.name = topic_name;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic '%s'.\n", topic.name);
        return 1;
    }

    /* Publish the 'work' status every 5 seconds */
    unsigned i = 0;
    while (1) {
        char *status = (char *)status_messages[i++ % 2];
        printf("Publishing: %s\n", status);

        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}