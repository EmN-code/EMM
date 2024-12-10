#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <esp_wifi.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <esp_wifi.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define EMCUTE_PORT     (1883U)
#define BUFFER_SIZE     (64U)
#define TOPIC_NAME      "state"
#define WILL_TOPIC      "will/topic"
#define WILL_MSG        "I am going offline"
#define DEFAULT_PRIO    (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static emcute_topic_t topic;

static void *pub_thread(void *arg) {
    (void)arg;

    while (1) {
        const char *state_msg = "work";
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("error: unable to publish data");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    gw.port = EMCUTE_PORT;

    int res = inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    if (res != 1) {
        puts("error: unable to parse IP");
        return 1;
    }
    
    if (emcute_con(&gw, true, WILL_TOPIC, WILL_MSG, strlen(WILL_MSG), 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to register topic");
        return 1;
    }

    thread_create(stack, sizeof(stack), DEFAULT_PRIO, 0, pub_thread, NULL, "pub_thread");

    while (1) {
        // Main thread can perform other tasks or sleep
        xtimer_sleep(60);
    }

    return 0;
}
