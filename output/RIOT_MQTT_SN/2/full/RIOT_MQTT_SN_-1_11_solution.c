#include <net/sock/util.h>
#include <MQTTClient.h>
#include <stdatomic.h>
#include <dht_params.h>
#include <thread.h>
#include <net/gcoap.h>
#include <xtimer.h>
#include <net/ipv6/addr.h>
#include <esp_wifi.h>
#include <stdio.h>
#include <stdbool.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <stdlib.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <string.h>
#include <arpa/inet.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <ztimer.h>
#include <assert.h>
#include <stddef.h>
#include <mutex.h>

#define EMCUTE_ID          "board"
#define TOPIC_NAME         "status"
#define BROKER_ADDRESS     "47.102.103.1"
#define BROKER_PORT        1883
#define PUBLISH_INTERVAL   (5LU * US_PER_SEC)

static kernel_pid_t emcute_thread_pid;
static char emcute_stack[THREAD_STACKSIZE_DEFAULT];

static emcute_topic_t topic;

void *emcute_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = BROKER_PORT };
    if (inet_pton(AF_INET, BROKER_ADDRESS, &gw.addr.ipv4) == 0) {
        puts("Error: unable to parse gateway address\n");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: unable to connect to gateway\n");
        return NULL;
    }

    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char *message = "work";
        if (emcute_pub(&topic, message, strlen(message), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish message\n");
        }
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    emcute_thread_pid = thread_create(emcute_stack, sizeof(emcute_stack),
                                      THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                      emcute_thread, NULL, "emcute");

    emcute_run(BROKER_PORT, EMCUTE_ID);

    return 0;
}