#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/af.h>
#include <kernel_types.h>
#include <msg.h>
#include <net/emcute.h>
#include <thread.h>

#define EMCUTE_ID          1
#define EMCUTE_PORT        1883
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_STACKSIZE   THREAD_STACKSIZE_LARGE
#define BROKER_IPv4_ADDR   "47.102.103.1"  // Use IPv4 address for broker
#define BROKER_PORT        1883

static emcute_topic_t topic;
static char stack[EMCUTE_STACKSIZE];
static msg_t queue[8];

static emcute_topic_t _topic;
static char _server_addr[] = "47.102.103.1"; // broker IPv4 address

static void *_emcute_thread(void *arg) {
    (void)arg;
    emcute_host_t server = {
        .family = AF_INET,
        .ipv4 = { 47, 102, 103, 1 },
        .port = EMCUTE_PORT
    };

    if (emcute_con(&server, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[FAILED] unable to connect to MQTT-SN broker\n");
        return NULL;
    }
    printf("[CONNECTED] to gateway \n");
    
    _topic.name = "state";
    _topic.id = 0;

    if (emcute_reg(&_topic) != EMCUTE_OK) {
        printf("[FAILED] unable to acquire topic ID\n");
        return NULL;
    }
    printf("[REGISTERED] topic available\n");

    while (1) {
        char data[] = "work or not work";
        if (emcute_pub(&_topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[FAILED] unable to publish data\n");
        } else {
            printf("[PUBLISHED] data sent\n");
        }
        xtimer_sleep(5);
    }
    
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 8);

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, THREAD_CREATE_STACKTEST,
                  _emcute_thread, NULL, "emcute_thread");

    return 0;
}
