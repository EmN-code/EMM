#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "net/emcute.h"
#include "core.h" // assuming this is a necessary header for RIOT

#define EMCUTE_ID "emcute-client"
#define EMCUTE_PORT 1883

static emcute_topic_t topic;

static char state[10] = "work";
static msg_t queue[8];

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    char *in = (char *)data;
    printf("Received publication: %.*s\n", (int)len, in);
}

static void *publish_thread(void *arg)
{
    (void)arg;
    msg_init_queue(queue, 8);
    while (1) {
        /* publish messages every 5s */
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void)
{
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    /* initialize Emcute */
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    /* connect to MQTT-SN broker */
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to broker\n");
        return 1;
    }

    /* register publish-service */
    emcute_reg(&topic);

    /* start publishing thread */
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "pub");    

    return 0;
}