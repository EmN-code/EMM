#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <arpa/inet.h>

#include "emcute.h"

#define EMCUTE_ID         "board"
#define EMCUTE_PRIO       (THREAD_PRIORITY_MAIN - 1)

#define INTERVAL          (5U * US_PER_SEC)
#define MQTT_SN_ADDR      "47.102.103.1"
#define MQTT_SN_PORT      1883

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *emcute_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_SN_PORT };

    if (inet_pton(AF_INET, MQTT_SN_ADDR, &gw.addr.ipv4) != 1) {
        printf("error: unable to parse IP address\n");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("error: unable to connect to gateway\n");
        return NULL;
    }

    /* setup topic */
    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("error: unable to register topic\n");
        return NULL;
    }

    char message[] = "work";

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("error: unable to publish data\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(stack, ARRAY_SIZE(stack));
    puts("MQTT-SN example application\n");
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute_thread");
    return 0;
}