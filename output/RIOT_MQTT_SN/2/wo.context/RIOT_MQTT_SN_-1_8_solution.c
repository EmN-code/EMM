#include <stdio.h>
#include <string.h>
#include <net/network.h>  // Add this import for 'inet_pton'
#include <net/emcute.h>
#include <xtimer.h>
#include "thread.h"

#define EMCUTE_ID          "station"
#define EMCUTE_PORT        1883
#define STACK_SIZE         (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_IDLE)

static char stack[STACK_SIZE];
static msg_t queue[8];  // Increase size if needed

static emcute_topic_t topic;

static sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

void on_pub(const emcute_topic_t *topic, void *data, size_t len)
{
    (void)topic;
    printf("Received publication on topic: %s, data: %s\n", topic->name, (char *)data);
}

void *mqtt_thread(void *arg)
{
    (void)arg;
    emcute_sub_t sub;
    int res;

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        puts("Connection failed");
        return NULL;
    }
    else {
        puts("Connected successfully");
    }

    topic.name = "state";
    emcute_reg(&topic);

    while(1) {
        char *data = "work";
        emcute_pub(&topic, data, strlen(data), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4); // Changed 'ipv6' to 'ipv4'
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}