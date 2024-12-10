#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include <stdio.h>
#include <string.h>

#define MQTT_TOPIC "work_status"
#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *_mqtt_pub(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    while (1) {
        /* Publish "work" status every 5 seconds */
        t.name = MQTT_TOPIC;
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("Error: Unable to register topic");
            continue;
        }

        if (emcute_pub(&t, "work", strlen("work"), flags) != EMCUTE_OK) {
            puts("Error: Unable to publish data");
        } else {
            puts("Data published");
        }

        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _mqtt_pub, NULL, "mqtt");

    msg_init_queue(queue, sizeof(queue)/sizeof(msg_t));
    
    /* Connect to MQTT-SN broker */
    sock_udp_ep_t gw = { .family = AF_INET6, .port = SERVER_PORT };
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, SERVER_ADDR) == NULL) {
        puts("Error: Unable to parse broker address");
        return 1;
    }

    if (emcute_sock(&gw) != EMCUTE_OK) {
        puts("Error: Unable to connect to broker");
        return 1;
    }

    puts("Connected to broker successfully");
    
    /* Continue working while the publish thread performs operations */
    while (1) {}

    return 0;
}
