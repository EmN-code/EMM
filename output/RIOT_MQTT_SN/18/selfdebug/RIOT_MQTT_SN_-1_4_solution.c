#include <xtimer.h>
#include <string.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_PERIOD (5 * US_PER_SEC)  // 5 seconds
#define TOPIC_NAME "device/state"
#define MAX_TOPICS 1

static emcute_topic_t topics[MAX_TOPICS];
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

void mqttsn_pub(void) {
    const char *message = "work or not work";
    emcute_topic_t *t = &topics[0];
    t->name = TOPIC_NAME;
    if (emcute_reg(t) != EMCUTE_OK) {
        puts("Failed to register topic");
        return;
    }
    if (emcute_pub(t, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
        puts("Failed to publish data");
    } else {
        puts("Data published");
    }
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);

    msg_init_queue(queue, ARRAY_SIZE(queue));
    emcute_run(THREAD_CREATE_STACKTEST, stack, sizeof(stack), "emcute", EMCUTE_PRIO, NULL);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to broker");
        return 1;
    }

    while (1) {
        mqttsn_pub();
        xtimer_sleep(PUBLISH_PERIOD / US_PER_SEC);
    }

    return 0;
}