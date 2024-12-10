#include <ztimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <stdio.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_TOPIC "device/state"

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];

void *pub_thread(void *arg) {
    (void)arg;

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = MQTT_TOPIC;
    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("[error] unable to register topic\n");
        return NULL;
    }

    while (1) {
        const char* data = "work";
        if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
            printf("[error] unable to publish data\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    msg_init_queue(queue, 8);

    if (inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4) == 0) {
        printf("[error] unable to parse IPv4 address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[error] unable to connect to gateway\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, pub_thread, NULL, "pub_thread");

    emcute_run(MQTT_BROKER_PORT, MQTT_CLIENT_ID);

    return 0;
}