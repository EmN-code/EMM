#include <ztimer.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <arpa/inet.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <sys/socket.h>

#define EMCUTE_ID          1U
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_MAXLEN       32U
#define MQTT_MSG_SIZE      128

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static emcute_topic_t topic;
static char mqtt_msg[MQTT_MSG_SIZE];

#define SERVER_ADDRESS     "47.102.103.1"
#define SERVER_PORT        1883

void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDRESS, &gw.addr.ipv4);

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    /* Register the topic "state" */
    strncpy(topic.name, "state", sizeof(topic.name));
    emcute_reg(&topic);

    while (1) {
        snprintf(mqtt_msg, sizeof(mqtt_msg), "%s", "work");
        emcute_pub(&topic, mqtt_msg, sizeof(mqtt_msg), 0);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  mqtt_thread, NULL, "mqtt_thread");

    return 0;
}