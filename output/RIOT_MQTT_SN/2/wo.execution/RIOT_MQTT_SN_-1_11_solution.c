#include "string.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "MQTTClient.h"
#include "stdbool.h"
#include "xtimer.h"
#include "dht.h"
#include "assert.h"
#include "time_units.h"
#include "ztimer.h"
#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gcoap.h"
#include "thread.h"
#include "timex.h"
#include "stdatomic.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "mutex.h"
#include "paho_mqtt.h"
#include "net/emcute.h"
#include "stdio.h"
#include "net/sock/udp.h"
#include "stdlib.h"
#include "esp_wifi.h"
#include "stddef.h"
#include "dht_params.h"

#define EMCUTE_PORT     (1883U)
#define MQTT_BROKER_IP  "47.102.103.1"
#define PUB_INTERVAL    (5U) /* seconds */

#define TOPIC_NAME      "test/state"
#define PAYLOAD_WORK    "work"
#define PAYLOAD_NOTWORK "not work"

static emcute_topic_t topic;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *publisher(void *arg) {
    (void)arg;
    while (1) {
        const char *pub_msg = PAYLOAD_WORK;  // Or logic to determine state

        if (emcute_pub(&topic, pub_msg, strlen(pub_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            printf("Error: unable to publish message\n");
        }

        ztimer_sleep(ZTIMER_SEC, PUB_INTERVAL);
    }
    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) != 1) {
        printf("Error: unable to parse broker IP\n");
        return 1;
    }

    puts("Connecting to MQTT-SN broker...");
    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to broker\n");
        return 1;
    }

    puts("Registering topic...");
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }
    topic.id = EMCUTE_TOPIC_UNDEF;

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher, NULL, "publisher");

    return 0;
}