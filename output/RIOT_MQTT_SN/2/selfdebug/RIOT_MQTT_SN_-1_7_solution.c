#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/af.h>
#include <net/emcute.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EMCUTE_ID           "gertrud"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic_name[TOPIC_MAXLEN];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("### received publication for topic '%s' [%i]\n",
            topic->name, topic->id);
}

static int pub(char *topic_name, char *data, int qos) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    if (qos == 1) {
        flags = EMCUTE_QOS_1;
    }

    /* step 1: get topic id */
    t.name = topic_name;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    /* step 2: publish data */
    if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("[pub] published %s on topic %s\n", data, topic_name);

    return 0;
}

int main(void) {
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));

    // Initialize Emcute
    emcute_run(QUEUE_SIZE, queue, EMCUTE_PRIO);

    struct sockaddr_in gw = { .sin_family = AF_INET, .sin_port = htons(1883) };
    inet_pton(AF_INET, "47.102.103.1", &gw.sin_addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    strncpy(topic_name, "test", TOPIC_MAXLEN);

    while (1) {
        pub(topic_name, "work or not work", 0);
        xtimer_sleep(5);
    }

    return 0;
}