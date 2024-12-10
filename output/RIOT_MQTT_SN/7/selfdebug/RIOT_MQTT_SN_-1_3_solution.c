#include "net/sock/udp.h"
#include "net/sock/util.h"
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "emcute.h"

#define EMCUTE_ID "mqtt-sn"
#define DEVICE_ID "esp32_device"
#define STACKSIZE (THREAD_STACKSIZE_MAIN)
#define TOPIC_NAME "state/topic"
#define SERVER "47.102.103.1"
#define PORT (1883U)

static char stack[STACKSIZE];
static msg_t queue[8];

static void *sender(void *arg) {
    (void)arg;

    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;

    t.name = TOPIC_NAME;
    t.id = 0;

    while(1) {
        char *state_msg = "work";
        printf("[MQTT-SN] Publishing: %s\n", state_msg);

        if (emcute_reg(&t) != EMCUTE_OK) {
            puts("[MQTT-SN] Error registering topic\n");
        }

        if (emcute_pub(&t, state_msg, strlen(state_msg), flags) != EMCUTE_OK) {
            puts("[MQTT-SN] Error publishing\n");
        }

        xtimer_sleep(5);
    }

    return NULL;
}

static void on_pub(const emcute_topic_t *topic, void *data, size_t len) {
    char *in = (char *)data;
    printf("[MQTT-SN] Received publication from topic '%s': %.*s\n", topic->name, len, in);
}

int main(void) {
    puts("MQTT-SN client application for ESP32\n");

    msg_init_queue(queue, ARRAY_SIZE(queue));

    sock_udp_ep_t gw = {
        .family = AF_INET,
        .port = PORT
    };

    if (inet_pton(AF_INET, SERVER, &gw.addr.ipv4) != 1) {
        puts("[MQTT-SN] Unable to parse IP address\n");
        return 1;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("[MQTT-SN] Unable to connect to server\n");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");

    return 0;
}