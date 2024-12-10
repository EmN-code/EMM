#include <net/ipv4/addr.h>
#include <msg.h>
#include <xtimer.h>
#include <emcute.h>
#include <net/gnrc.h>
#include <stdio.h>

#define EMCUTE_PORT (1883U)
#define BROKER_IP "47.102.103.1"
#define PUB_INTERVAL (5U) // in seconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static const sock_udp_ep_t broker = {
    .family = AF_INET,
    .port = EMCUTE_PORT
};

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(EMCUTE_PORT, "esp32_client");
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");
    
    uint8_t addr[4];
    inet_pton(AF_INET, BROKER_IP, &addr);

    if (emcute_con((sock_udp_ep_t *)&addr, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Failed to connect to broker\n");
        return 1;
    }

    topic.name = "device/state";
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return 1;
    }

    while (1) {
        const char *state_msg = "work"; // Assume this state can toggle between "work" and "not work"
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Failed to publish message\n");
        } else {
            printf("Published: %s\n", state_msg);
        }
        xtimer_sleep(PUB_INTERVAL);
    }
    return 0;
}