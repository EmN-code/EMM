#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shell.h"
#include "msg.h"
#include "emcute.h"

#define EMCUTE_ID           "esp32"
#define EMCUTE_PORT         (1883U)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];
static char *topics_sub[NUMOFSUBS];

static emcute_sub_t subscriptions[NUMOFSUBS];
static emcute_topic_t topics_pub[NUMOFSUBS];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    char *in = (char *)data;

    printf("### Response: "%.*s" (from topic '%s') ###\n", len, in, topic->name);
}

static int connect_broker(void)
{
    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = EMCUTE_PORT
    };
    uint8_t addr[] = {47, 102, 103, 1};
    memcpy(&gw.addr.ipv6, addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to MQTT-SN broker\n");
        return 1;
    }

    return 0;
}

static int publish(char *topic, char *data, int flags)
{
    unsigned len = strlen(data);

    emcute_topic_t t;
    t.name = topic;
    if (emcute_reg(&t) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&t, data, len, flags) != EMCUTE_OK) {
        printf("error: unable to publish data to topic: %s\n", topic);
        return 1;
    }

    printf("Published %s to topic %s\n", data, topic);

    return 0;
}

int main(void)
{
    puts("RIOT MQTT-SN example\n");

    memset(subscriptions, 0, (NUMOFSUBS * sizeof(emcute_sub_t)));
    memset(topics_sub, 0, (NUMOFSUBS * sizeof(char*))) ;

    emcute.init();

    if(connect_broker() != 0) {
        printf("Could not connect to the broker\n");
        return 1;
    }

    const char *state_msg[2] = {"work", "not work"};
    unsigned int state = 0;

    while (1) {
        publish("esp32/state", state_msg[state % 2], EMCUTE_QOS_0);
        state++;
        xtimer_sleep(5);
    }

    return 0;
}