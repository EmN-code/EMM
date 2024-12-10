#include "ztimer.h"
#include "net/emcute.h"
#include "unistd.h"
#include "arpa/inet.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define IP_ADDR             "47.102.103.1"
#define PORT                1883
#define PUB_TOPIC_NAME      "device/status"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = PORT };
    inet_pton(AF_INET, IP_ADDR, &gw.addr);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        return NULL;
    }

    memset(&topic, 0, sizeof(emcute_topic_t));
    strncpy(topic.name, PUB_TOPIC_NAME, sizeof(topic.name));

    if (emcute_reg(&topic) != EMCUTE_OK) {
        return NULL;
    }

    while (1) {
        const char *status = "work"; 
        if (emcute_pub(&topic, status, strlen(status), EMCUTE_QOS_1) != EMCUTE_OK) {
            return NULL;
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(stack, sizeof(stack), EMCUTE_PRIO, "_emcute");
    emcute_thread(NULL);
    return 0;
}