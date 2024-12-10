
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC_NAME "state"
#define MSG_SIZE 15

static char msg_buf[MSG_SIZE];
static emcute_topic_t topic;

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        strcpy(msg_buf, "work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);

        strcpy(msg_buf, "not work");
        if (emcute_pub(&topic, msg_buf, strlen(msg_buf), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(NULL, 512, THREAD_PRIORITY_MAIN - 1, 0, emcute_thread, NULL, "emcute");

    return 0;
}
