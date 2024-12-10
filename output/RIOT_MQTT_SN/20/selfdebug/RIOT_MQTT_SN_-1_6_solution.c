#include <xtimer.h>
#include <string.h>
#include <stdlib.h>
#include <net/ipv4/addr.h>
#include <net/sock/udp.h>
#include "emcute.h"

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define NUMOFSUBS           (16U)
#define TOPIC_MAXLEN        (64U)
#define MQTT_INTERVAL       (5U) // 5 seconds
#define MQTT_IPV4_ADDR      "47.102.103.1"
#define MQTT_PORT           (1883U)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *mqtt_thread(void *arg) {
    emcute_topic_t t;
    int res;
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_PORT };

    if (inet_pton(AF_INET, MQTT_IPV4_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address");
        return NULL;
    }

    res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("[mqtt] error: connection failed: %d\n", res);
        return NULL;
    }

    t.name = "state";

    while (1) {
        char *payload = "work";

        t.id = 0; // Assume topic is pre-registered

        res = emcute_pub(&t, payload, strlen(payload), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[mqtt] error: unable to publish data: %d\n", res);
        }

        xtimer_sleep(MQTT_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt");
    return 0;
}