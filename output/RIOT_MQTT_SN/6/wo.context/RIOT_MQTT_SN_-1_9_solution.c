#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "msg.h"
#include "net/af.h"
#include "net/sock/udp.h"
#include "emcute.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_TOPIC_MAXLEN (64U)
#define SERVER_ADDR         "47.102.103.1"
#define MESSAGE_INTERVAL    (5U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void on_pub(struct emcute_sub_t *sub, const emcute_topic_t *topic, void *data, size_t len) {
    (void)sub;
    (void)topic;
    printf("Received publication: %s\n", (char *)data);
}

static int publish(const char *topic_name, const char *data) {
    topic.name = topic_name;
    topic.id = 1;
    unsigned flags = EMCUTE_QOS_0;

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain a topic ID");
        return 1;
    }

    if (emcute_pub(&topic, data, strlen(data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic '%s'\n", topic_name);
        return 1;
    }

    printf("Published %s on topic %s\n", data, topic_name);
    return 0;
}

static int connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET6, SERVER_ADDR, &gw.addr);
    gw.port = EMCUTE_PORT;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway");
        return 1;
    }
    puts("Successfully connected to gateway");
    return 0;
}

int main(void) {
    msg_init_queue(queue, 8);
    memset(stack, 0, sizeof(stack));
    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    if (connect() != 0) {
        return 1;
    }

    while (1) {
        publish("device/state", "work");
        sleep(MESSAGE_INTERVAL);
    }

    return 0;
}