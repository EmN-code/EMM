#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/af.h>
#include <net/emcute.h>
#include <msg.h>
#include <arpa/inet.h> // Include for inet_pton

#define EMCUTE_PORT     (1883U)
#define NUMOFSUBS       (16U)
#define TOPIC_MAXLEN    (64U)

static emcute_sub_t subscriptions[NUMOFSUBS];
static emcute_topic_t topic;
static char topic_max_len[TOPIC_MAXLEN];

static void on_pub(const emcute_topic_t *t, void *data, size_t len) {
    (void)t;
    printf("Received publication: %.*s\n", (int)len, (char *)data);
}

static int pub(const char *topic_name, const char *data) {
    unsigned flags = EMCUTE_QOS_0;
    topic.name = topic_name;
    topic.id = 0;

    int res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return res;
    }

    res = emcute_pub(&topic, data, strlen(data), flags);
    if (res != EMCUTE_OK) {
        puts("Error: unable to publish data");
        return res;
    }

    return 0;
}

int main(void) {
    sock_udp_ep_t gw = {.family = AF_INET, .port = EMCUTE_PORT};
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return res;
    }

    while (1) {
        pub("state", "work or not work");
        xtimer_sleep(5);
    }

    return 0;
}