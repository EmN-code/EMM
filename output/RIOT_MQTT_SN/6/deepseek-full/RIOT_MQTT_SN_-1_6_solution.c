
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <msg.h>
#include <thread.h>
#include <unistd.h>

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_STATE "work or not work"
#define MSG_QUEUE_SIZE 10
#define PRIORITY THREAD_PRIORITY_MAIN - 1
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

static char stack[STACK_SIZE];
static kernel_pid_t mqtt_thread;
static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

void *mqtt_worker(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to MQTT-SN broker");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        emcute_discon();
        return NULL;
    }

    while (1) {
        if (emcute_pub(&topic, MSG_STATE, sizeof(MSG_STATE) - 1, 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    mqtt_thread = thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_worker, NULL, "mqtt_worker");

    return 0;
}
