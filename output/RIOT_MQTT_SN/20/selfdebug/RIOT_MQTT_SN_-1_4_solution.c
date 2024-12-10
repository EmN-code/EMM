#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PORT     (1883U)
#define SERVER_ADDR     "47.102.103.1"
#define PUBLISH_INTERVAL 5   // seconds

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(SERVER_ADDR, EMCUTE_PORT, EMCUTE_ID_LIFETIME, EMCUTE_DEFAULT_PORT);
    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    printf("Published state: \"%s\"\n", (char *)data);
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    strcpy(topic.name, "state");
    emcute_reg(&topic);

    while (1) {
        const char *state = "work or not work";
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}