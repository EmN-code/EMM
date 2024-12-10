#include <stdio.h>
#include <string.h>
#include <emcute.h>
#include <thread.h>

#define EMCUTE_ID           "mymqtt"
#define EMCUTE_PORT         1883
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received publication: %.*s\n", (int)len, (char *)data);
}

static void *emcute_thread(void *arg)
{
    int res;
    emcute_sub_t subscriptions[1];

    subscriptions[0].topic.name = topic.name;
    subscriptions[0].cb = on_pub;

    emcute_init(queue, sizeof(queue), EMCUTE_PRIO, EMCUTE_ID);
    res = emcute_con("47.102.103.1", EMCUTE_PORT);
    if (res != EMCUTE_OK) {
        printf("[FAILED to connect to broker] \n");
        return NULL;
    }

    char *message;

    while (1) {
        message = "work";  // Simulate "work" state. Change to "not work" as needed.
        res = emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[FAILED to publish data] \n");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    topic.name = "state";
    topic.id = 0;

    return 0;
}