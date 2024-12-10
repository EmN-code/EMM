#include <xtimer.h>
#include <string.h>
#include <msg.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "device_id"
#define EMCUTE_PORT         (1883U)
#define MQTT_BROKER_IP      "47.102.103.1"
#define PUB_MSG             "work"
#define PUB_INTERVAL        (5U)
#define TOPIC_NAME          "device/status"
#define TOPIC_MAXLEN        (64U)
#define THREAD_STACK_SIZE   (THREAD_STACKSIZE_DEFAULT)

static char stack_pub[THREAD_STACK_SIZE];
static msg_t queue[8];

static emcute_topic_t topic;

void *pub_thread(void *arg) {
    (void)arg;
    uint8_t flags = EMCUTE_QOS_0;

    while (1) {
        if (emcute_pub(&topic, PUB_MSG, strlen(PUB_MSG), flags) != EMCUTE_OK) {
            puts("[error] unable to publish");
        } else {
            puts("[info] published");
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return NULL;
}

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error on conn\n");
        return -1;
    }
    return 0;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    puts("[info] Starting emcute client\n");
    if (connect_broker() != 0) {
        return 1;
    }

    strcpy(topic.name, TOPIC_NAME);
    topic.id = 0;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error on reg\n");
        return 1;
    }

    puts("[info] creating pub thread\n");
    thread_create(stack_pub, sizeof(stack_pub), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return 0;
}