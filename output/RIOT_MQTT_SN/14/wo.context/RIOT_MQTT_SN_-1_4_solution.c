#include <stdlib.h>
#include <string.h>
#include <net/sock.h>
#include <net/emcute.h>
#include <net/af.h>
#include <xtimer.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT    1883
#define PUB_INTERVAL   5    // in seconds

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;
static char *topic_name = "device/state";

int main(void) {
    /* Setup emcute task */
    msg_t queue[8];
    msg_init_queue(queue, 8);
    
    emcute_run(THREAD_PRIORITY_MAIN - 1, stack, sizeof(stack), "emcute");

    /* Setup broker endpoint */
    sock_udp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_ADDRESS, &remote.addr.ipv4);

    /* Connect to broker */
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Connection failed with code %d\n", res);
        return 1;
    }
    
    /* Register topic "device/state" */
    topic.name = topic_name;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("Registration failed with code %d\n", res);
        return 1;
    }

    /* Publish loop */
    const char *states[] = { "work", "not work" };
    unsigned count = 0;

    while (1) {
        int idx = count++ % 2;

        res = emcute_pub(&topic, states[idx], strlen(states[idx]), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publish failed with code %d\n", res);
        } else {
            printf("Published state: %s\n", states[idx]);
        }

        xtimer_sleep(PUB_INTERVAL);
    }

    /* Disconnect from the broker */
    res = emcute_discon();
    if (res != EMCUTE_OK) {
        printf("Disconnection failed with code %d\n", res);
    }

    return 0;
}