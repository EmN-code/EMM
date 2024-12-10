#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "event/thread.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "MQTTClient.h"
#include "net/sock/udp.h"
#include "thread.h"
#include "arpa/inet.h"

#include <stdio.h>
#include <string.h>

#define EMCUTE_PORT         (1883U)
#define MQTT_BROKER_IP      "47.102.103.1"
#define TOPIC_NAME          "state"
#define SLEEP_INTERVAL      (5 * US_PER_SEC)  // 5 seconds

static sock_udp_ep_t remote = { .family = AF_INET, .port = EMCUTE_PORT };
static emcute_topic_t topic;

static void *mqtt_thread(void *arg) {
    (void)arg;
    char *states[2] = {"work", "not work"};
    
    while (1) {
        for (int i = 0; i < 2; ++i) {
            int res = emcute_pub(&topic, states[i], strlen(states[i]), EMCUTE_QOS_0);
            if (res != EMCUTE_OK) {
                printf("error: unable to publish data (%d)\n", res);
            }
            xtimer_sleep(SLEEP_INTERVAL);
        }
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN publisher application
");

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4) != 1) {
        puts("error: unable to parse IPv4 address
");
        return 1;
    }

    puts("Attempting to connect to MQTT-SN gateway
");
    int res = emcute_con(&remote, true, NULL, NULL, 0, 0);
    if (res != EMCUTE_OK) {
        printf("error: unable to connect to gateway (%d)\n", res);
        return 1;
    }

    topic.name = TOPIC_NAME;
    res = emcute_reg(&topic);
    if (res != EMCUTE_OK) {
        printf("error: unable to register topic (%d)\n", res);
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}