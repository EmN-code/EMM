#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/ipv6/addr.h>
#include <net/emcute.h>

#define EMCUTE_PORT     1883
#define MQTT_STATE_MSG  "work"
#define PUBLISH_INTERVAL (5U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t topic;

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
}

int main(void) {
    int res;
    emcute_sub_t subscription;
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = EMCUTE_PORT };

    // Convert IP
    inet_pton(AF_INET, "47.102.103.1", &gw.addr.ipv4);

    // Initialize message queue
    msg_init_queue(queue, ARRAY_SIZE(queue));

    // Initialize emcute
    memset(&subscription, 0, sizeof(emcute_sub_t));
    subscription.topic.name = MQTT_STATE_MSG;
    subscription.cb = on_pub;

    res = emcute_sub(&subscription, EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error subscribing: %d\n", res);
        return 1;
    }

    while (1) {
        emcute_pub(&topic, MQTT_STATE_MSG, strlen(MQTT_STATE_MSG), EMCUTE_QOS_0);
        xtimer_sleep(5);
    }

    return 0;
}