#include "sys/socket.h"
#include "stdlib.h"
#include "stdio.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "net/sock/util.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "thread.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "netinet/in.h"

#define EMCUTE_ID          (0)
#define MQTT_BROKER_IP    "47.102.103.1"
#define MQTT_BROKER_PORT  1883
#define PUB_TOPIC         "device/state"
#define WORK_STATE        "work"
#define NOT_WORK_STATE    "not work"

static emcute_topic_t t;

void *mqtt_publish_thread(void *arg) {
    (void)arg;
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr);

    while (1) {
        if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
            printf("Connection to MQTT-SN broker failed, retrying...\n");
            thread_sleep(1000000);
            continue;
        }

        printf("Connected to MQTT-SN broker\n");
        break;
    }

    while (1) {
        const char *state = WORK_STATE;  // Set your logic to determine the actual state
        if (emcute_pub(&t, state, sizeof(state), 0) == EMCUTE_OK) {
            printf("State published: %s\n", state);
        } else {
            printf("Failed to publish state\n");
        }
        thread_sleep(5000000);
    }

    return NULL;
}

int main(void) {
    t.name = PUB_TOPIC;
    t.id = 1234;  // Assign a valid topic ID

    if (emcute_reg(&t) != EMCUTE_OK) {
        printf("Failed to register publish topic\n");
        return -1;
    }

    printf("Registered and ready to publish\n");

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, mqtt_publish_thread, NULL, "mqtt_pub");

    return 0;
}