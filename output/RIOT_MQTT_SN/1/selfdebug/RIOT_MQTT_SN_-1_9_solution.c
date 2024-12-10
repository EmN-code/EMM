#include <ztimer.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/ipv4/addr.h>
#include <net/emcute.h>
#include <arpa/inet.h>

#define EMCUTE_ID           "mqttsn"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_TOPIC_MAXLEN (32U)
#define EMCUTE_BUFSIZE      (128U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define BROKER_IP           "47.102.103.1"
#define BROKER_PORT         (1883U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

void on_pub(const emcute_topic_t *topic, const void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // Handle incoming message if necessary
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    int res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (res == 0) {
        puts("Error: invalid broker address");
        return 1;
    }

    puts("Connecting to broker ...");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to broker");
        return 1;
    }
    puts("Connection successful");

    topic.name = "state";
    topic.id = 0;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic");
        return 1;
    }
    puts("Topic registered successfully");

    while (1) {
        char payload[] = "work";
        if (emcute_pub(&topic, payload, sizeof(payload), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}