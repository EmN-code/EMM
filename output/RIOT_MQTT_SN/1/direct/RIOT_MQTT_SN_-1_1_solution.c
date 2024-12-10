#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <net/emcute.h>

#define EMCUTE_PRIO         (THREAD_PRIORITY_MAIN - 1)
#define MQTT_SN_BROKER_IP   "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define INTERVAL            (5U * US_PER_SEC)  // Publish every 5 seconds
#define TOPIC_NAME          "device/state"

static char stack[THREAD_STACKSIZE_DEFAULT];
static emcute_topic_t t;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_PRIO);
    return NULL;
}

int main(void) {
    int res;
    char *message = "work";

    msg_init_queue(queue, EMCUTE_QUEUE_SIZE);

    if (thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute") <= 0) {
        puts("[Error] unable to start emcute thread");
        return 1;
    }

    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_SN_BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_SN_BROKER_IP);
    if (emcute_con(&gw) != EMCUTE_OK) {
        puts("[Error] unable to connect to broker");
        return 1;
    }
    puts("[Success] connected to broker");

    /* Publish state every 5 seconds */
    while (1) {
        xtimer_sleep(INTERVAL / US_PER_SEC);

        t.name = TOPIC_NAME;
        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("[Error] unable to register topic");
            continue;
        }

        if (emcute_pub(&t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("[Error] unable to publish message");
        } else {
            puts("[Success] message published");
        }
    }

    return 0;
}