#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <net/emcute.h>
#include <msg.h>
#include <event/thread.h>
#include <thread.h>

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define CLIENT_ID       "esp32_client"
#define MQTT_TOPIC      "device/state"
#define MSG_QUEUE_SIZE  8

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];
static char mqtt_stack[THREAD_STACKSIZE_DEFAULT];

static emcute_topic_t topic;

void *mqtt_thread(void *arg) {
    (void)arg;
    emcute_run(BROKER_PORT, CLIENT_ID);
    return NULL;
}

void *publish_thread(void *arg) {
    (void)arg;

    while (1) {
        const char *state = "work"; // or "not work"
        emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);  
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    
    return NULL;
}

int main(void) {
    struct sockaddr_in broker_addr;

    broker_addr.sin_family = AF_INET;
    broker_addr.sin_port = htons(BROKER_PORT);
    broker_addr.sin_addr.s_addr = (47 << 24) | (102 << 16) | (103 << 8) | 1; // Manual conversion to IPv4 address

    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_thread");

    topic.name = MQTT_TOPIC;
    emcute_reg(&topic);

    thread_create(mqtt_stack, sizeof(mqtt_stack), THREAD_PRIORITY_MAIN - 1, 0, publish_thread, NULL, "publish_thread");

    return 0;
}