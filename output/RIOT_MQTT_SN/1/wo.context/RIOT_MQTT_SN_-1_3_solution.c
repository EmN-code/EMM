#include <stdio.h>
#include "net/sock/udp.h"
#include "net/ipv6/addr.h"
#include "emcute.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define PUBLISH_INTERVAL 5
#define EMCUTE_PORT 1883  // Default port for emcute
#define EMCUTE_ID "esp32"

static emcute_sub_t subscriptions[1];
static char stack[THREAD_STACKSIZE_DEFAULT];

static int pub_state(void) {
    const char *state = "work"; // Or "not work" depending on your application logic
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;
    topic.id = 0; // will be set by registration

    int res = emcute_pub(&topic, state, strlen(state), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data\n");
        return 1;
    }
    else {
        printf("Published state: %s\n", state);
    }
    
    return 0;
}

static void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(EMCUTE_PORT, EMCUTE_ID); // Only two arguments are passed
    return NULL;
}

static int setup_connection(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };
    ipv6_addr_t addr;
    if (ipv6_addr_from_str(&addr, MQTT_BROKER_IP) == NULL) {
        printf("Error: unable to parse broker IP\n");
        return 1;
    }
    memcpy(&gw.addr.ipv6, &addr, sizeof(addr));

    int res = emcute_con(&gw, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("Error: Unable to connect to MQTT-SN broker\n");
        return 1;
    }
    return 0;
}

int main(void) {
    msg_init_queue(_main_msg_queue, sizeof(_main_msg_queue) / sizeof(_main_msg_queue[0]));

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                  emcute_thread, NULL, "emcute");

    if (setup_connection() != 0) {
        return 1;
    }

    emcute_topic_t t;
    t.name = MQTT_TOPIC;
    t.id = 0;  // will be set by the register

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Error: unable to obtain topic ID\n");
        return 1;
    }

    while (1) {
        pub_state();
        xtimer_sleep(PUBLISH_INTERVAL);
    }

    return 0;
}