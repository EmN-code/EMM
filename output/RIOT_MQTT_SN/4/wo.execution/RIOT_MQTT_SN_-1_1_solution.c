#include "xtimer.h"
#include "thread.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "net/emcute.h"

#define EMCUTE_PORT     1883
#define EMCUTE_ID       "esp32_client"
#define EMCUTE_TOPIC    "state"
#define EMCUTE_BROKER   "47.102.103.1"
#define INTERVAL        (5U * 1000000U)

static emcute_sub_t subscription;
static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    puts("Message published");
}

static void *thread_pub(void *arg) {
    (void)arg;

    while (1) {
        const char *message = "work";
        emcute_msg_t msg;

        msg.data = (void *)message;
        msg.len = strlen(message);
        msg.topic = topic.id;
        msg.flags = EMCUTE_MSGFLAG_PERSIST;

        if (emcute_pub(&msg) != EMCUTE_OK) {
            puts("publish failed");
        } else {
            printf("Published state: %s\n", message);
        }

        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    /* initialize our subscription structure */
    memset(&subscription, 0, sizeof(emcute_sub_t));
    subscription.cb = on_pub;

    /* initialize the topic to publish on */
    memset(&topic, 0, sizeof(emcute_topic_t));
    topic.name = EMCUTE_TOPIC;

    /* start the emcute thread to connect to broker */
    if (emcute_sock_is_connected() || emcute_con(NULL, EMCUTE_BROKER) != EMCUTE_OK) {
        puts("unable to connect to broker");
        return 1;
    }

    /* subscribe and block waits until connected */
    if (emcute_sub(&subscription, EMCUTE_ID) != EMCUTE_OK) {
        puts("unable to subscribe to broker");
        return 1;
    }

    /* publish thread creation */
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_pub, NULL, "pub_thread");

    /* let the main thread sleep and see thread_pub handle publishing */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}