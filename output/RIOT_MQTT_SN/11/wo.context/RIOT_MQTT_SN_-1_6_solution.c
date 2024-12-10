#define CONFIG_ASYMCUTE_KEEPALIVE_PING 60 
#define CONFIG_ASYMCUTE_TOPIC_MAXLEN 32 

#include <string.h>
#include "xtimer.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"

#define BROKER_PORT 1883
#define BROKER_ADDRESS "47.102.103.1"
#define MQTT_TOPIC "device/state"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static void *emcute_thread(void *arg) {
    (void)arg;
    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(CONFIG_ASYMCUTE_DEFAULT_PORT, stack, sizeof(stack));
    return NULL;
}

int main(void) {
    // Start the emcute thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, emcute_thread, NULL, "emcute");

    sock_udp_ep_t gw = {.family = AF_INET, .port = BROKER_PORT};

    // Convert the broker address from text to binary form
    int res = inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);
    if (res != 1) {
        puts("Error: Unable to convert broker address");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway");
        return 1;
    }

    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to obtain topic ID");
        return 1;
    }

    while (1) {
        const char *state_msg = "work";  // Example state message
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish data");
        }

        xtimer_sleep(5);  // Wait for 5 seconds
    }

    return 0;
}