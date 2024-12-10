#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <emcute.h>
#include <periph/rtc.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define INTERVAL 5

static emcute_topic_t topic;

void publish_state(const char *state) {
    emcute_msg_t msg;
    msg.data = (void *)state;
    msg.len = strlen(state);
    msg.type = EMCUTE_MSG_TYPE_PUBLISH;
    msg.flags = EMCUTE_MSG_FLAG_NONE;

    if (emcute_pub(&topic, &msg) != 0) {
        printf("Error: unable to publish data\n");
    } else {
        printf("Published: %s\n", state);
    }
}

int connect_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = BROKER_PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, BROKER_IP);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != 0) {
        printf("Error: unable to connect to broker\n");
        return -1;
    }

    printf("Connected to broker at %s\n", BROKER_IP);

    /* Set topic ID */
    topic.name = MQTT_TOPIC;
    if (emcute_reg(&topic) != 0) {
        printf("Error: unable to register topic\n");
        return -1;
    }
    return 0;
}

int main(void) {
    puts("Starting MQTT-SN example\n");

    if (connect_broker() != 0) {
        return 1;
    }

    while (1) {
        publish_state("work or not work");
        xtimer_sleep(INTERVAL);
    }

    return 0;
}