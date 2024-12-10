#include <string.h>
#include <stdio.h>
#include "msg.h"
#include "thread.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "device1"
#define EMCUTE_PORT         1883
#define SERVER_ADDRESS      "47.102.103.1"
#define STATE_TOPIC_ID      1

#define STATE_WORK          "work"
#define STATE_NOT_WORK      "not work"

static emcute_topic_t state_topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

void publish_state(const char *state) {
    emcute_topic_t topic;
    memset(&topic, 0, sizeof(topic));
    topic.name = "state/topic";
    topic.id = STATE_TOPIC_ID;

    while (emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0) != EMCUTE_OK) {
        printf("Error: unable to publish message\n");
    }

    printf("Published state: %s\n", state);
}

void *mqttsn_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET6 };
    gw.port = EMCUTE_PORT;
    inet_pton(AF_INET6, SERVER_ADDRESS, &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return NULL;
    }
    printf("Connected to gateway at %s\n", SERVER_ADDRESS);

    state_topic.name = "state/topic";
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        publish_state(STATE_WORK);
        xtimer_sleep(5);
        publish_state(STATE_NOT_WORK);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    puts("Starting MQTT-SN example\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqttsn_thread, NULL, "mqttsn");

    return 0;
}
