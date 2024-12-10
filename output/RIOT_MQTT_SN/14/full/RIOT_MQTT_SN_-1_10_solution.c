#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "msg.h"

#define EMCUTE_PORT        (1883U)
#define EMCUTE_ID          (0U)
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS          (16U)
#define TOPIC_MAXLEN       (64U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_sub_t subs[NUMOFSUBS];
static char topics[NUMOFSUBS][TOPIC_MAXLEN];
static emcute_topic_t topic;

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received publication!");
}

static void *emcute_thread(void *arg)
{
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void)
{
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));

    kernel_pid_t pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                                     emcute_thread, NULL, "emcute");
    if (pid <= 0) {
        printf("[Error] Unable to start emcute thread!");
        return 1;
    }

    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    if (inet_pton(AF_INET, "47.102.103.1", &gw.addr) == 0) {
        printf("[Error] Unable to parse gateway address!");
        return 1;
    }

    while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[Error] Unable to connect to gateway!");
        xtimer_sleep(1);
    }

    strcpy(topics[0], "state");
    topic.name = topics[0];

    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[Error] Unable to register topic!");
        return 1;
    }

    while (1) {
        const char *pub_msg = "work";
        if (emcute_pub(&topic, pub_msg, strlen(pub_msg), 0) != EMCUTE_OK) {
            printf("[Error] Unable to publish data!");
        }
        xtimer_sleep(5);
    }
    return 0;
}