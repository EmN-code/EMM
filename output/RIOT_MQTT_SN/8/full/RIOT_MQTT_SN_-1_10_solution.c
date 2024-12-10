#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "ztimer.h"
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/socket.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "mbox.h"
#include "msg.h"
#include "unistd.h"
#include "dht.h"
#include "dht_params.h"
#include "mutex.h"
#include "MQTTClient.h"
#include "arpa/inet.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5U * 1000000U)

static emcute_topic_t topic;
char stack[THREAD_STACKSIZE_DEFAULT];
sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };

static void *mqtt_thread(void *arg) {
    (void)arg;
    int rc;
    
    printf("Starting MQTT-SN publisher example\n");

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse IPv4 address\n");
        return NULL;
    }

    emcute_run(stack, sizeof(stack), "mqtt_client");

    rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
    while (rc != EMCUTE_OK) {
        printf("Connection attempt to MQTT-SN broker failed. Trying again...\n");
        ztimer_sleep(ZTIMER_MSEC, 1000);
        rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
    }
    printf("Connected to MQTT-SN broker.\n");

    topic.name = "state";
    emcute_reg(&topic);

    while (1) {
        const char *state_msg = (rand() % 2) ? "work" : "not work";
        printf("Publishing state: %s\n", state_msg);

        rc = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        while (rc != EMCUTE_OK) {
            printf("Failed to publish state. Retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
            rc = emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0);
        }
        ztimer_sleep(ZTIMER_USEC, PUBLISH_INTERVAL);
    }

    emcute_discon();
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");
    return 0;
}