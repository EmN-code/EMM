#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <emcute.h>
#include <board.h>
#include <net/ipv6/addr.h>
#include <net/sock/async.h>
#include "xtimer.h"

#define EMCUTE_PORT         1883
#define MQTT_SN_SERVER      "47.102.103.1"
#define TOPIC_NAME          "state"
#define PUB_INTERVAL        (5U * US_PER_SEC)

static emcute_topic_t _topic;

static int publish(const char *state) {
    emcute_msg_t msg;
    unsigned flags = EMCUTE_QOS_0;  // QoS level 0

    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = flags;

    return emcute_publish(&_topic, &msg);
}

int main(void) {
    gpio_init(LED0_PIN, GPIO_OUT);
    memset(&_topic, 0, sizeof(_topic));
    
    puts("Setting up network interface...");
    if (netif_add(NULL, NULL, NULL, NULL) < 0) {
        puts("Could not add network interface");
        return 1;
    }

    /* start the emcute thread */
    emcute_setup();

    /* connect to the broker */
    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = EMCUTE_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_SN_SERVER);

    if (emcute_con(gw, false, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Could not connect to gateway");
        return 1;
    }

    /* setup the topic we will publish to */
    _topic.name = TOPIC_NAME;
    if (emcute_reg(&_topic) != EMCUTE_OK) {
        puts("Could not obtain topic ID");
        return 1;
    }

    puts("Publishing state...");

    while (1) {
        /* publish message */
        int res = publish("working" /* or "not working" based on state */);
        if (res != EMCUTE_OK) {
            puts("Error while publishing");
        }
        
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}