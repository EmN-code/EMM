#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <thread.h>
#include <msg.h>
#include <timex.h>
#include <ztimer.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <net/emcute.h>

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUB_INTERVAL    (5U)
#define TOPIC_NAME      "esp32/status"

static kernel_pid_t emcute_thread;
static char emcute_stack[THREAD_STACKSIZE_MAIN];
static msg_t emcute_queue[8];
static emcute_topic_t status_topic;
static char status_stack[THREAD_STACKSIZE_MAIN];

static void *emcute_thread_handler(void *arg) {
    (void)arg;
    msg_init_queue(emcute_queue, ARRAY_SIZE(emcute_queue));

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: Unable to parse MQTT broker address");
        return NULL;
    }

    emcute_run(EMCUTE_PORT, "esp32");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: Could not connect to MQTT broker");
        return NULL;
    }

    strncpy(status_topic.name, TOPIC_NAME, sizeof(status_topic.name));
    if (emcute_reg(&status_topic) != EMCUTE_OK) {
        puts("ERROR: Unable to register topic");
        return NULL;
    }

    while (1) {
        const char *work_msg = "work";
        if (emcute_pub(&status_topic, work_msg, strlen(work_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("ERROR: Unable to publish data");
        }
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    emcute_thread = thread_create(emcute_stack, sizeof(emcute_stack),
                                  THREAD_PRIORITY_MAIN - 1, 0,
                                  emcute_thread_handler, NULL, "emcute");
    while (1) {
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
        puts("Main loop is active");
    }
    return 0;
}