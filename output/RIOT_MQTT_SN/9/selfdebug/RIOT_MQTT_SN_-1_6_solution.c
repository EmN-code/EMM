#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <thread.h>

// Recommended stack size for an example application
#define MQTT_THREAD_STACKSIZE THREAD_STACKSIZE_LARGE
#define MQTT_THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_PRIO (THREAD_PRIORITY_MAIN - 2)
#define NUMOFSUBS 1
#define TOPIC_MAXLEN 64

static char stack[MQTT_THREAD_STACKSIZE];
static emcute_topic_t topic;

static void *mqtt_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = 1883 };  // gateway address
    ipv4_addr_t addr;
    inet_pton(AF_INET, "47.102.103.1", &addr);
    memcpy(gw.addr.ipv4, &addr, sizeof(addr));

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to gateway\n");
        return NULL;
    }

    pub_topic = (emcute_topic_t){ .name = "state" };
    if (emcute_reg(&pub_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return NULL;
    }

    topic.id = pub_topic.id;

    while (1) {
        char message[] = "work";  // or "not work"
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish\n");
        } else {
            printf("Published: %s\n", message);
        }
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}