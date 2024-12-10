#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "msg.h"
#include "mutex.h"
#include "time_units.h"
#include "thread.h"
#include "event/thread.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "kernel_defines.h"
#include "unistd.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "netinet/in.h"
#include "net/ipv6/addr.h"

#define EMCUTE_PORT         (1883U)
#define MQTT_BUFFER_SIZE    (128)
#define MQTT_TOPIC_MAXLEN   (64)
#define THREAD_STACKSIZE    (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY     (THREAD_PRIORITY_MAIN - 1)

static char stack[THREAD_STACKSIZE];
static msg_t queue[8];

static emcute_topic_t mqtt_topic;
static char mqtt_topic_name[MQTT_TOPIC_MAXLEN];

void *publish_thread(void *arg) {
    (void)arg;
    while(1) {
        const char *message = "work";
        emcute_pub(&mqtt_topic, message, strlen(message), 0);
        ztimer_sleep(ZTIMER_SEC, 5);  // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    snprintf(mqtt_topic_name, sizeof(mqtt_topic_name), "state_topic");  // Ensure topic name will fit
    mqtt_topic.name = mqtt_topic_name;
    mqtt_topic.id = 0;

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: Unable to connect to gateway.");
        return 1;
    }

    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        puts("Error: Unable to register topic.");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0,
                  publish_thread, NULL, "publish_thread");

    emcute_run(EMCUTE_PORT, "mqtt-client");

    return 0;
}