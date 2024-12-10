
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"

#define EMCUTE_PORT       1883
#define EMCUTE_ID         "riot_esp32"
#define TOPIC_NAME        "state"
#define PUBLISH_INTERVAL  (5 * 1000000U)  // 5 seconds in microseconds

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    gw.family = AF_INET;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    char payload[20];
    while (1) {
        strcpy(payload, "work");
        if (emcute_pub(&topic, payload, strlen(payload), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    thread_create(emcute_stack, sizeof(emcute_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
