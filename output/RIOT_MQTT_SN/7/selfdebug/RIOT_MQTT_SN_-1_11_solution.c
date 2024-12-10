#include <net/emcute.h>
#include <net/ethernet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include "xtimer.h"

#define EMCUTE_ID          ("esp32_mqttsn_client")
#define EMCUTE_PRIO        THREAD_PRIORITY_MAIN - 1
#define MSG_QUEUE_SIZE     (8)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static emcute_topic_t topic;

static const uint8_t MAX_RETRIES = 10;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *str = (char *)data;
    printf("Topic received (id: %d), data length: %u",
           topic->id, (unsigned)len);
    printf("Data: %s\n", str);
}

static int _send_message(const char* data) {
    unsigned flags = EMCUTE_QOS_0;
    if (emcute_pub(&topic, data, strlen(data), flags) != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return 1;
    }
    printf("Published %s on topic %d\n", data, topic.id);
    return 0;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    printf("MQTTSN client initialized\n");

    uint8_t retries = 0;
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };
    gw.addr.ipv4[0] = 47;
    gw.addr.ipv4[1] = 102;
    gw.addr.ipv4[2] = 103;
    gw.addr.ipv4[3] = 1;

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: couldn't connect to broker.");
        return 1;
    }

    topic.name = "esp32/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic.");
        return 1;
    }

    while(1) {
        if (_send_message("work") != 0) {
            printf("Retry count: %d\n", retries);
            if (++retries >= MAX_RETRIES) {
                puts("Max retries reached.");
                break;
            }
        }
        xtimer_sleep(5);
    }

    emcute_discon();
    return 0;
}