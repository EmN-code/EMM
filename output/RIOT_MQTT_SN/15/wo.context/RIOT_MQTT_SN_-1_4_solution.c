#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <net/sock/udp.h>

#define EMCUTE_ID          "mqttsn-proxy"
#define EMCUTE_PRIO        (THREAD_PRIORITY_MAIN - 1)
#define TOPIC_NAME         "device_state"
#define MQTT_BROKER_IP     "47.102.103.1"
#define MQTT_BROKER_PORT   1883
#define STATE_MSG          "work"
#define NON_STATE_MSG      "not work"

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_topic_t topic;

typedef struct {
    kernel_pid_t sender_pid;
    uint16_t type;
    union {
        void *ptr;
        uint32_t value;
    } content;
} msg_t;

void *emcute_thread(void *arg) {
    (void)arg;
    emcute_run(MQTT_BROKER_PORT, EMCUTE_ID);
    return NULL;
}

static int connect_to_broker(sock_udp_ep_t *gw) {
    while (1) {
        int res = emcute_con(gw, true, NULL, NULL, 0, 0);
        if (res == EMCUTE_OK) {
            puts("Successfully connected to broker");
            break;
        } else {
            printf("Connection failed: %d, retrying...\n", res);
            xtimer_sleep(1);
        }
    }
    return 0;
}

static int publish_state(bool state) {
    const char *msg = state ? STATE_MSG : NON_STATE_MSG;
    int res = emcute_pub(&topic, msg, strlen(msg), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Failed to publish data: %d\n", res);
        return 1;
    }
    printf("Published %s state\n", msg);
    return 0;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    msg_t msg;

    thread_create(stack, sizeof(stack), EMCUTE_PRIO, 0, emcute_thread, NULL, "emcute");

    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    connect_to_broker(&gw);

    topic.name = TOPIC_NAME;
    emcute_reg(&topic);

    while (1) {
        publish_state(true);
        xtimer_sleep(5);
    }

    return 0;
}