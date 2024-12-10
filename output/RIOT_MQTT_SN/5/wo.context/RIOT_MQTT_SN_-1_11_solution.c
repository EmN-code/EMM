#include <stdio.h>
#include <net/sock/udp.h>
#include <emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC)

static emcute_sub_t emcute_subscriptions[16];

static emcute_topic_t emcute_topics[16];

static char topic_name[] = "state";
static const char *states[] = {"work", "not work"};
static size_t state_idx = 0;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("Received publication: %.*s\n", len, (char *)data);
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };

    size_t res = inet_pton(AF_INET, BROKER_IP, &gw.addr.ipv4);
    if (res != 1) {
        puts("ERROR: Could not parse broker address");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    msg_init_queue(queue, ARRAY_SIZE(queue));

    /* Initialize emCute */
    emcute_run(BROKER_PORT, "esp32-client");

    /* Connect to the MQTT-SN gateway */
    int ret = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (ret != EMCUTE_OK) {
        printf("ERROR: Could not connect to gateway [%d]\n", ret);
        return 1;
    }

    /* Register topic */
    emcute_topics[0].name = topic_name;
    ret = emcute_reg(&emcute_topics[0]);
    if (ret != EMCUTE_OK) {
        printf("ERROR: Could not register topic [%d]\n", ret);
        return 1;
    }

    while (1) {
        /* Publish state */
        state_idx = (state_idx + 1) % 2;  // Toggle state
        ret = emcute_pub(&emcute_topics[0], states[state_idx], strlen(states[state_idx]), EMCUTE_QOS_0);
        if (ret != EMCUTE_OK) {
            printf("ERROR: Could not publish [%d]\n", ret);
        }
        else {
            printf("Published: %s\n", states[state_idx]);
        }

        /* Delay */
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}