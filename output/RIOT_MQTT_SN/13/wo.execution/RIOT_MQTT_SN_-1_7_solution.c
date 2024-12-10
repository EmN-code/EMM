#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_SN_PORT        (1883U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subscriptions[NUMOFSUBS];
static char topic[TOPIC_MAXLEN];

static void *pub_thread(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    
    strcpy(topic, "state");
    t.name = topic;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *data = "work";
        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* initialize emcute */
    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(&t, 0, sizeof(emcute_topic_t));

    emcute_con(&(sock_udp_ep_t){ .family = AF_INET, .addr.ipv4 = { 0 }, .port = MQTT_SN_PORT }, true, NULL, NULL, 0, 0);

    if (inet_pton(AF_INET, "47.102.103.1", &(socket_udp_ep_t.addr.ipv4)) != 1) {
        printf("error: unable to parse gateway address\n");
        return 1;
    }

    /* start publisher thread */
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, pub_thread, NULL, "pub_thread");

    return 0;
}