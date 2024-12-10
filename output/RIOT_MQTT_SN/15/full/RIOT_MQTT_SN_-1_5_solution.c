#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "stddef.h"
#include "sys/socket.h"
#include "esp_system.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "mutex.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"

#define EMCUTE_ID       "mqtt-sensor"
#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME      "state"
#define MESSAGE_WORK    "work"
#define MESSAGE_NOT_WORK "not work"

static emcute_topic_t mqtt_topic;
static sock_udp_ep_t gw = {.family = AF_INET, .port = MQTT_BROKER_PORT};

static int connect_to_broker(void) {
    int res;
    if (ipv6_addr_from_str((ipv6_addr_t *)&gw.addr.ipv6, MQTT_BROKER_IP) == NULL) {
        printf("Error setting gateway address\n");
        return -1;
    }

    while ((res = emcute_con(&gw, true, NULL, NULL, 0, 0)) != EMCUTE_OK) {
        printf("Failed to connect to gateway. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}

static int publish_state(const char *state) {
    unsigned flags = EMCUTE_QOS_0;

    while (emcute_pub(&mqtt_topic, state, strlen(state), flags) != EMCUTE_OK) {
        printf("Failed to publish message. Retrying...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}

int main(void) {
    printf("Starting MQTT-SN publisher with emcute\n");

    if (connect_to_broker() < 0) {
        return -1;
    }

    mqtt_topic.name = TOPIC_NAME;
    if (emcute_reg(&mqtt_topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        return -1;
    }

    while (1) {
        publish_state(MESSAGE_WORK);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}