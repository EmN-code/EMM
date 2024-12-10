#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <net/ipv6/addr.h>
#include <net/sock/udp.h>
#include <timex.h>
#include <ztimer.h>
#include <net/emcute.h>

#define BROKER_ADDRESS "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/status"
#define PUBLISH_INTERVAL (5U * 1000U) // Publish every 5 seconds

static msg_t _main_msg_queue[8];
static emcute_topic_t _topic;

static void _on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    (void)data;
    (void)len;
    // We do not need to handle incoming publishes in this example
}

static void _publish_state(void) {
    const char *state = "work"; // Example state, can change to "not work"
    emcute_pub(&_topic, state, strlen(state), EMCUTE_QOS_0);
}

int main(void) {
    msg_init_queue(_main_msg_queue, 8);

    // Set up the address struct
    sock_udp_ep_t gw = { .family = AF_INET,
                         .port = BROKER_PORT };

    inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4);

    // Connect to UDP
    emcute_con(&gw, true, NULL, NULL, 0, EMCUTE_QOS_0);

    // Register the topic
    _topic.name = MQTT_TOPIC;
    emcute_reg(&_topic);

    // Publish loop
    while (1) {
        _publish_state();
        ztimer_sleep(ZTIMER_MSEC, PUBLISH_INTERVAL);
    }

    return 0;
}