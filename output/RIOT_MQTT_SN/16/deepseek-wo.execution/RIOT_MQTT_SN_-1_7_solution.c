
#include <net/emcute.h>
#include <msg.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stdio.h>
#include <arpa/inet.h>

#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024
#define PRIORITY 1

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[STACK_SIZE];

static void *mqtt_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(1883) };
    inet_aton("47.102.103.1", &gw.addr.ipv4);

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

    while (1) {
        const char *state = "work";
        if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
            puts("Error: unable to publish message");
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    kernel_pid_t pid = thread_create(stack, sizeof(stack), PRIORITY, 0, mqtt_thread, NULL, "mqtt_thread");

    if (pid <= 0) {
        puts("Error: unable to create MQTT thread");
        return 1;
    }

    return 0;
}
