#include <ztimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <msg.h>
#include <thread.h>
#include <arpa/inet.h>
#include <stdio.h>

#define EMCUTE_PORT        (1883U)
#define MQTT_BROKER_IP     "47.102.103.1"
#define PUB_TOPIC          "state"
#define PUB_INTERVAL       (5U)
#define STACK_SIZE         (THREAD_STACKSIZE_DEFAULT)
#define NUM_SUBS           (1U)

static char stack[STACK_SIZE];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    char *message = "work";
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        puts("Error: Unable to parse IP");
        return NULL;
    }

    emcute_con(&gw, true, NULL, NULL, 0, 0);

    topic.name = PUB_TOPIC;
    emcute_reg(&topic);

    while (1) {
        if (emcute_pub(&topic, message, strlen(message), 0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }
        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(queue, ARRAY_SIZE(queue));
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    emcute_run(EMCUTE_PORT, "device_id");
    return 0;
}
