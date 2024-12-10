#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <net/af.h>
#include <net/conn/ip.h>
#include <net/emcute.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define EMCUTE_ID           "esp32"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (1U)
#define TOPIC_MAXLEN        (64U)

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscriptions[NUMOFSUBS];
static emcute_topic_t publication_topic;

static const char *mqtt_sn_host = "47.102.103.1";
static const uint16_t mqtt_sn_port = 1883;

void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("Received publication on topic: %s [%i] \n", topic->name, (int)topic->id);
    printf("Data: %s\n", in);
}

static int connect(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = mqtt_sn_port };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, mqtt_sn_host) == NULL) {
        puts("error: unable to parse destination address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }

    return 0;
}

static int publish(char* pub_data) {
    unsigned flags = EMCUTE_QOS_0;

    publication_topic.name = "esp32/work-state";

    if (emcute_reg(&publication_topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&publication_topic, pub_data, strlen(pub_data), flags) != EMCUTE_OK) {
        printf("error: unable to publish data\n");
        return 1;
    }

    printf("Published %s\n", pub_data);

    return 0;
}

int main(void) {
    puts("MQTT-SN Publisher\n");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    emcute_run(mqtt_sn_port, EMCUTE_ID);

    if (connect() != 0) {
        puts("error: could not connect to broker\n");
        return 1;
    }

    while (1) {
        publish("work");
        xtimer_sleep(5);
        publish("not work");
        xtimer_sleep(5);
    }

    return 0;
}