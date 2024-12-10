#include <stdint.h>
#include <stdio.h>
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "string.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_SN_SERVER      "47.102.103.1"
#define MQTT_SN_PORT        (1883)

static char emcute_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[NUMOFSUBS];
static emcute_topic_t topic;

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_SN_PORT, "esp32_client");
    return NULL;
}

static int publish(const char *topic_name, const char *data) {
    topic.name = topic_name;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("error: unable to obtain topic ID\n");
        return 1;
    }

    if (emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("error: unable to publish data\n");
        return 1;
    }

    printf("Published %s on topic %s\n", data, topic.name);
    return 0;
}

int main(void) {
    memset(&topic, 0, sizeof(topic));

    msg_init_queue(queue, ARRAY_SIZE(queue));

    thread_create(emcute_stack, sizeof(emcute_stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_PORT };
    if (inet_pton(AF_INET, MQTT_SN_SERVER, &gw.addr.ipv4) != 1) {
        puts("error: unable to parse gateway address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("error: unable to connect to gateway\n");
        return 1;
    }
    puts("Connected to MQTT-SN gateway\n");

    while (1) {
        publish("esp32/status", "work");
        xtimer_sleep(5);
    }

    return 0;
}
