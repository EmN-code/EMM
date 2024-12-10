#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <architecture.h>

#define EMCUTE_PORT        (1883U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define SERVER_IP          "47.102.103.1"
#define MQTT_TOPIC         "device/state"
#define MSG_TYPE           "work or not work"
#define LOOP_DELAY         (5000000U) /* microseconds */

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void publish_state(void)
{
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = MQTT_TOPIC;

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[Failed] unable to register topic
");
        return;
    }

    while (1) {
        int res = emcute_pub(&t, MSG_TYPE, strlen(MSG_TYPE), flags);
        if (res != EMCUTE_OK) {
            printf("[Failed] publish message: %d
", res);
        }
        usleep(LOOP_DELAY);
    }
}

int main(void)
{
    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[Failed] unable to connect to MQTT-SN gateway
");
        return 1;
    }

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  (thread_task_func_t)publish_state, NULL, "publish_thread");

    return 0;
}