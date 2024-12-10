
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMCUTE_PORT 1883
#define EMCUTE_ID "riot_esp32"
#define TOPIC_NAME "state"
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_sub_t sub;
static sock_udp_ep_t gw = { .port = EMCUTE_PORT };

static void publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the broker

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Error: unable to register topic (%d)\n", res);
        return;
    }

    const char *state = states[state_index];
    res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish message (%d)\n", res);
    } else {
        printf("Published: %s\n", state);
    }

    state_index = (state_index + 1) % 2;
}

static void *emcute_thread(void *arg) {
    (void)arg;

    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res != 1) {
        printf("Error: unable to convert IP address\n");
        return NULL;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to connect to MQTT-SN broker (%d)\n", res);
        return NULL;
    }

    while (1) {
        xtimer_usleep(PUBLISH_INTERVAL);
        publish_state();
    }

    return NULL;
}

int main(void) {
    puts("RIOT MQTT-SN example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    return 0;
}
