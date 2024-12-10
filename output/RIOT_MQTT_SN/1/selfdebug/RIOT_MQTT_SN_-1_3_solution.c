#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <od.h>

#define EMCUTE_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define PUB_TOPIC_NAME  "state"
#define PUB_INTERVAL    (5U*XTIMER_COEFF) // 5 seconds

#ifndef EMCUTE_ID
#define EMCUTE_ID       "board"
#endif

static sock_udp_ep_t gw = {
    .port = EMCUTE_PORT
};

static void _publish_state(void)
{
    // Prepare message
    const char *states[] = {"work", "not work"};
    char *message = (char *)states[rand() % 2];
    size_t len = strlen(message);

    // Prepare topic
    emcute_topic_t topic;
    topic.name = PUB_TOPIC_NAME;
    emcute_reg(&topic);

    // Publish data
    int res = emcute_pub(&topic, message, len, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data [%i]\n", res);
    }
}

static void *_emcute_thread(void *arg)
{
    (void)arg;
    // Connect to broker
    gw.family = AF_INET;
    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to broker\n");
        return NULL;
    }
    printf("Connected to broker at %s\n", BROKER_ADDRESS);

    while (1) {
        _publish_state();
        xtimer_usleep(PUB_INTERVAL);
    }
    return NULL;
}

int main(void)
{
    // Start emcute thread
    static char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _emcute_thread, NULL, "emcute");

    // Run the main loop
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return 0;
}