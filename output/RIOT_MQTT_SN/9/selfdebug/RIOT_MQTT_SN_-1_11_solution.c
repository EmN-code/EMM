#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define EMCUTE_ID           "device"
#define EMCUTE_PORT         (1883U)
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define SLEEP_SEC           (5U)

static emcute_topic_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT];
static uint8_t buf[128];
static unsigned msg_count = 0;

sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };

static void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    printf("Received publication for topic '%s' [%i]: %.*s\n", 
            topic->name, topic->id, (int)len, (char *)data);
}

int main(void)
{
    puts("RIOT MQTT-SN example application");
    msg_init_queue(_recv_stack, ARRAY_SIZE(_recv_stack));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, 
                  (thread_task_func_t)emcute_thread, NULL, "emcute");

    // Connect to the broker
    char *addr_str = "47.102.103.1";
    inet_pton(AF_INET6, addr_str, &gw.addr.ipv6);
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }
    puts("Connected to MQTT-SN broker");

    // Subscribe to a topic
    subscriptions[0].cb = on_pub;
    subscriptions[0].name = "status";
    emcute_sub(&subscriptions[0], EMCUTE_QOS_0);
    puts("Subscribed to status topic");

    emcute_topic_t topic;
    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        msg_count++;
        const char *state_str = (msg_count % 2) ? "work" : "not work";
        emcute_pub(&topic, state_str, strlen(state_str), EMCUTE_QOS_0);
        printf("Published: %s
", state_str);
        xtimer_sleep(SLEEP_SEC);
    }
    return 0;
}