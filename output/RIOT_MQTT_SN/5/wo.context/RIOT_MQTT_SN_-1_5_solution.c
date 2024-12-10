#include <xtimer.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/af.h>
#include <string.h>

#define EMCUTE_ID          1
#define EMCUTE_PORT        1883
#define EMCUTE_BROKER_IPv4 "47.102.103.1"
#define TOPIC_NAME         "state"
#define MESSAGE            "work"

static emcute_sub_t _subscriptions[EMCUTE_ID];
static emcute_topic_t _topic;

int main(void) {
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };

    ipv4_addr_from_str((ipv4_addr_t *)&gw.addr.ipv4, EMCUTE_BROKER_IPv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to gateway");
        return 1;
    }

    puts("Connected to MQTT broker");

    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("Failed to register topic");
        return 1;
    }

    while (1) {
        if (emcute_pub(&_topic, MESSAGE, strlen(MESSAGE), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Failed to publish message");
        } else {
            puts("Message published");
        }
        xtimer_sleep(5);
    }

    return 0;
}