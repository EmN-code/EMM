#include "ztimer.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define MQTT_TOPIC_NAME "state/topic"
#define STATE_MSG_WORK "work"
#define STATE_MSG_NOTWORK "not work"

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static int _publish_state(const char *state) {
    return emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
}

static void *publish_thread(void *arg) {
    (void)arg;
    
    while (1) {
        if (_publish_state(STATE_MSG_WORK) != EMCUTE_OK) {
            printf("[ERROR] Unable to publish work state\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, publish_thread, NULL, "publish");

    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        printf("[ERROR] unable to parse IPv4 address\n");
        return -1;
    }

    msg_init_queue(queue, 8);
    emcute_run(THREAD_PRIORITY_MAIN - 2, "mqttsn-client");

    while (1) {
        printf("Connecting to %s\n", SERVER_ADDR);
        while (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }

        printf("Registering topic %s\n", MQTT_TOPIC_NAME);
        memset(&topic, 0, sizeof(emcute_topic_t));
        topic.name = MQTT_TOPIC_NAME;
        if (emcute_reg(&topic) != EMCUTE_OK) {
            printf("[ERROR] unable to obtain topic ID\n");
            return -1;
        }
    }

    return 0;
}