#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "paho_mqtt.h"
#include "net/gcoap.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "event/thread.h"
#include "mutex.h"
#include "esp_system.h"
#include "kernel_defines.h"
#include "net/emcute.h"
#include "dht_params.h"

#define SERVER_ADDR       "47.102.103.1"
#define SERVER_PORT       1883
#define PUB_INTERVAL_SEC  5
#define MQTT_TOPIC        "device/state"
#define STACKSIZE         (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];
static msg_t queue[8];

static void *publisher(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    t.name = MQTT_TOPIC;

    while (1) {
        char *data = "work"; // or "not work" depending on some condition

        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("Error: unable to obtain topic ID\n");
            continue;
        }

        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            puts("Error: unable to publish data\n");
        } else {
            puts("Published data successfully");
        }

        sleep(PUB_INTERVAL_SEC);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_ADDR, &gw.addr.ipv4) != 1) {
        puts("Error: unable to parse gateway address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, publisher, NULL, "publisher");

    return 0;
}