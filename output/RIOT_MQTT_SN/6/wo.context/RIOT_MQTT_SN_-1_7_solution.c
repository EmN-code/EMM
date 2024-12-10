#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "net/sock/udp.h"
#include "msg.h"
#include "ztimer.h"
#include "net/emcute.h"

#define BROKER_PORT 1883      // Define Broker port
#define EMCUTE_ID "esp32_client"  // Define client ID
#define TOPIC_NAME "esp32/status"  // Define topic name

static emcute_topic_t status_topic;
static char *status_topic_name = TOPIC_NAME;

void on_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;  // silence -Wunused-parameter
    printf("Received data: %.*s\n", (int)len, (char *)data);
}

int init_mqtt_sn(void) {
    // Set the broker address
    sock_udp_ep_t gw = {
        .family      = AF_INET6,
        .port        = BROKER_PORT
    };
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, "47.102.103.1");

    // Connect to broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: Unable to connect to the gateway\n");
        return -1;
    }

    // Register topic
    memset(&status_topic, 0, sizeof(emcute_topic_t));
    status_topic.name = status_topic_name;
    if (emcute_reg(&status_topic) != EMCUTE_OK) {
        printf("Error: Unable to register topic\n");
        return -1;
    }

    return 0;
}

int main(void) {
    emcute_run(BROKER_PORT, EMCUTE_ID);

    if (init_mqtt_sn() != 0) {
        return -1;
    }

    char *status_working = "work";
    char *status_not_working = "not work";
    bool state = true;

    while (1) {
        const char *message = state ? status_working : status_not_working;
        if (emcute_pub(&status_topic, message, strlen(message), 0) != EMCUTE_OK) {
            printf("Error: Unable to publish data\n");
        } else {
            printf("Published: %s\n", message);
        }
        state = !state;
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}