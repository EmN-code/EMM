
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/ipv4/addr.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ADDR "47.102.103.1"
#define TOPIC "state"
#define MSG "work"

static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    emcute_topic_t topic;
    if (emcute_reg(&topic, TOPIC) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, MSG, strlen(MSG), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
