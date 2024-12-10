#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/af.h>
#include <net/emcute.h>
#include <ztimer.h>

#define INTERVAL (5U) // publish interval in seconds
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define CONFIG_EMCUTE_ID "esp32-client"
#define CONFIG_EMCUTE_PORT (1883)
#define CONFIG_EMCUTE_DEFAULT_PORT (1883U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static const char *state_msg = "work";
static sock_udp_ep_t gw = { .family = AF_INET6, .port = CONFIG_EMCUTE_PORT };

typedef struct {
    sock_udp_t sock;    /**< socket number */
} my_network_t;

static void pub(void)
{
    unsigned flags = EMCUTE_QOS_0;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic
");
        return;
    }

    res = emcute_pub(&topic, state_msg, strlen(state_msg), flags);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data
");
        return;
    }

    printf("Published %s to topic %s
", state_msg, topic.name);
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(CONFIG_EMCUTE_DEFAULT_PORT, CONFIG_EMCUTE_ID);
    return NULL;
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    memset(&gw, 0, sizeof(gw));
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv6);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect to gateway
");
        return 1;
    }

    strcpy(topic.name, "sensor/state"); // set the topic name

    while (1) {
        pub();
        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000); // wait 5 seconds
    }

    return 0;
}