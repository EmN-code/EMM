#include <stdio.h>
#include "net/sock/udp.h"
#include "net/mqttsn.h"
#include "kernel_defines.h"

#define EMCUTE_ID           "device"
#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define EMCUTE_BUFSIZE      (128U)
#define EMCUTE_PORT         (1883U)

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    printf("[MQTT-SN] published "%s": %.*s\n", topic->name, (int)len, (char *)data);
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_sub_t subscription;
    memset(&subscription, 0, sizeof(subscription));
    subscription.topic.name = "state";
    subscription.cb = on_pub;
    
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);
    
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[MQTT-SN] error: unable to connect to gateway\n");
        return NULL;
    }

    topic.name = "state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("[MQTT-SN] error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *msg = "work";
        if (emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("[MQTT-SN] error: unable to publish data\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    puts("RIOT MQTT-SN example\n");
    emcute_run(EMCUTE_PORT, EMCUTE_BUFSIZE, EMCUTE_ID, stack, sizeof(stack), EMCUTE_PRIO);
    kernel_pid_t emcute_pid = thread_create(stack, sizeof(stack), EMCUTE_PRIO,
                             THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");
    return 0;
}