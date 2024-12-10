#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "msg.h"
#include "event/thread.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "mutex.h"
#include "dht_params.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "dht.h"
#include "time_units.h"
#include "timex.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define EMCUTE_CLIENT_ID    "riot_client"
#define EMCUTE_TOPIC_NAME   "device/state"
#define EMCUTE_MSG          "work"
#define EMCUTE_PORT         1883
#define EMCUTE_ADDRESS      "47.102.103.1"

static emcute_topic_t topic;
static emcute_sub_t subscription;

void on_msg(const emcute_topic_t *topic, void *data, size_t len) {
    (void)topic;
    printf("[Topic: %s] Received data (%d bytes): %.*s\n",
           topic->name, len, len, (char *)data);
}

void connect_to_broker(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, EMCUTE_ADDRESS, &gw.addr.ipv4);

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) == EMCUTE_OK) {
            printf("Connected to gateway at %s:%d\n", EMCUTE_ADDRESS, EMCUTE_PORT);
            break;
        } else {
            printf("Failed to connect, retrying...\n");
            xtimer_sleep(1);
        }
    }
}

void register_topic(void) {
    topic.name = EMCUTE_TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Failed to register topic\n");
        exit(1);
    }
}

void publish_state(void) {
    while (1) {
        if (emcute_pub(&topic, EMCUTE_MSG, strlen(EMCUTE_MSG), 0) != EMCUTE_OK) {
            printf("Publish failed\n");
        } else {
            printf("Published: %s\n", EMCUTE_MSG);
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    msg_init_queue(queue, sizeof(queue) / sizeof(msg_t));
    connect_to_broker();
    register_topic();
    publish_state();
    return 0;
}