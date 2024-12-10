#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "mutex.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define TOPIC_NAME "device/state" 

static void *mqtt_publisher(void *arg) {
    (void)arg;
    int res;
    emcute_topic_t topic;
    topic.name = TOPIC_NAME;

    while (1) {
        res = emcute_reg(&topic);
        if (res != EMCUTE_OK) {
            printf("Failed to register topic: %d\n", res);
            xtimer_sleep(1);
            continue;
        }
        printf("Topic registered\n");
        break;
    }

    while (1) {
        const char *state_msg = "work"; // Example state
        res = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publish failed: %d\n", res);
        } else {
            printf("Published: %s\n", state_msg);
        }
        xtimer_sleep(5);
    }
    return NULL;
} 

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT};
    ipv6_addr_from_str((ipv6_addr_t *)&gw.addr, MQTT_BROKER_IP);

    printf("Connecting to MQTT-SN broker %s:%d\n", MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Connection attempt failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Connected successfully\n");
            break;
        }
    }

    mqtt_publisher(NULL);

    return 0;
}