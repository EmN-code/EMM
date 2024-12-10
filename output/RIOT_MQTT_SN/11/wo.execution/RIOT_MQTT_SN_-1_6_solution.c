#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdio.h"
#include "sys/socket.h"
#include "stdbool.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "string.h"
#include "msg.h"
#include "esp_system.h"
#include "dht.h"
#include "dht_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "emcute.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "event/thread.h"
#include "kernel_defines.h"
#include "MQTTClient.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mbox.h"

#define EMCUTE_ID     "esp_node"
#define EMCUTE_PORT   1883
#define SERVER_IP     "47.102.103.1"
#define STATE_TOPIC   "state"
#define INTERVAL      5

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t state_topic;
static sock_udp_ep_t gw;

static const char* state_payload = "work";

void *mqtt_thread(void *arg) {
    (void)arg;

    emcute_run(EMCUTE_PORT, EMCUTE_ID);
    return NULL;
}

int main(void) {
    // Convert IP address
    if (inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4) != 1) {
        puts("ERROR: unable to parse IP address");
        return 1;
    }
    gw.family = AF_INET;
    gw.netif = SOCK_ADDR_ANY_NETIF;
    gw.port = EMCUTE_PORT;

    // Start MQTT-SN thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt_thread");

    // Connect to the MQTT-SN broker
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("ERROR: unable to connect to broker");
        return 1;
    }

    // Register a topic
    state_topic.name = STATE_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        puts("ERROR: unable to register topic");
        return 1;
    }

    while (1) {
        // Publish state every INTERVAL seconds
        int res = emcute_pub(&state_topic, state_payload, strlen(state_payload), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("ERROR: unable to publish message (%d)\n", res);
        } else {
            puts("State published");
        }

        ztimer_sleep(ZTIMER_MSEC, INTERVAL * 1000);
    }

    return 0;
}