#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>
#include <msg.h>

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)
#define MSG_BUFFER_SIZE (64)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[NUMOFSUBS];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];

static emcute_topic_t publish_topic;

void *emcute_thread(void *arg) {
    (void) arg;
    emcute_run(EMCUTE_PORT, "device_id");
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("### Received publication for topic '%s' [%d byte(s)]: %.*s\n",
           topic->name, (int)len, (int)len, (char *)data);
}

int main(void) {
    msg_init_queue(queue, NUMOFSUBS);
    memset(subscriptions, 0, sizeof(subscriptions));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    gw.family = AF_INET;
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    publish_topic.name = "state";
    if (emcute_reg(&publish_topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        const char *state = (rand() % 2 == 0) ? "work" : "not work";
        if (emcute_pub(&publish_topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return 0;
}
