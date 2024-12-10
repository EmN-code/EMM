#include <stdio.h>
#include <string.h>
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "xtimer.h"

#define BROKER_PORT     1883
#define BROKER_ADDRESS  "47.102.103.1"
#define DEFAULT_PUB_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;
static char *topic_name = "device/status";

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    
    while (1) {
        xtimer_sleep(DEFAULT_PUB_INTERVAL / US_PER_SEC);
        
        char *message = "work"; // Example message
        
        size_t len = strlen(message);
        emcute_pub(&topic, message, len, EMCUTE_QOS_0);
    }
    return NULL;
}

int main(void) {
    puts("Starting MQTT-SN example with emcute");

    emcute_topic_t topic;
    topic.name = topic_name;

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0,
                  emcute_thread, NULL, "emcute_thread");

    sock_udp_ep_t gw = {
        .family = AF_INET6,
        .port = BROKER_PORT
    };

    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, BROKER_ADDRESS) == NULL) {
        printf("Error: unable to parse broker address %s\n", BROKER_ADDRESS);
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to the broker");
        return 1;
    }

    if (emcute_sub(&topic, EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Error: unable to subscribe to topic");
        return 1;
    }

    puts("MQTT-SN setup complete");
    return 0;
}
