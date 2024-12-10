#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "net/emcute.h"

#define EMCUTE_ID           "ESP32_client"
#define EMCUTE_PORT         1883
#define EMCUTE_ADDRESS      "47.102.103.1"
#define MQTT_TOPIC          "state"
#define MSG_TYPE_PUBLISH    (0x1337)
#define PUBLISH_INTERVAL    (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static int publish_state(const char *state) {
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = MQTT_TOPIC;
    t.id = 0;

    int res = emcute_reg(&t);
    if (res != EMCUTE_OK) {
        printf("error: unable to obtain topic ID (%d)\n", res);
        return res;
    }

    res = emcute_pub(&t, state, strlen(state), flags);
    if (res != EMCUTE_OK) {
        printf("error: unable to publish data (%d)\n", res);
        return res;
    }

    printf("published: %s\n", state);
    return 0;
}

static void *mqtt_pub_thread(void *arg) {
    msg_init_queue(queue, sizeof(queue) / sizeof(queue[0]));
    
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };    
    inet_pton(AF_INET, EMCUTE_ADDRESS, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to [%s]:%d\n", EMCUTE_ADDRESS, EMCUTE_PORT);
        return NULL;
    }

    while (1) {
        const char *state = "work";
        publish_state(state);
        xtimer_sleep(PUBLISH_INTERVAL / 1000000U);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub");
    return 0;
}
