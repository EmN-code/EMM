#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <msg.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define NUMOFTOPICS         (16U)
#define MESSAGE_INTERVAL    (5U) // in seconds
#define BROKER_PORT         (1883U)
#define BROKER_ADDRESS      "47.102.103.1"
#define TOPIC_NAME          "device/state"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_ADDRESS) == NULL) {
        printf("Error: unable to parse broker address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }
    return 0;
}

static int register_topic(const char *name) {
    topic.name = name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }
    return 0;
}

static void *publish_thread(void *arg) {
    (void)arg;
    while (1) {
        int res = emcute_pub(&topic, "work", strlen("work"), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Error: unable to publish data\n");
        }
        xtimer_sleep(MESSAGE_INTERVAL);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));

    if (connect_broker() != 0) {
        return 1;
    }

    if (register_topic(TOPIC_NAME) != 0) {
        return 1;
    }

    if (thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, publish_thread, NULL, "publish") <= 0) {
        printf("Error: unable to create publish thread\n");
        return 1;
    }

    emcute_run(BROKER_PORT, "state_publisher");

    return 0;
}