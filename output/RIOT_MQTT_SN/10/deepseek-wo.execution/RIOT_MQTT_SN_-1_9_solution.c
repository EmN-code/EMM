
#include "net/ipv6/addr.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "time_units.h"
#include "xtimer.h"
#include "esp_system.h"
#include "thread.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"
#include "mutex.h"
#include "stdio.h"
#include "dht_params.h"
#include "stdatomic.h"
#include "ztimer.h"
#include "dht.h"
#include "esp_wifi.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "net/sock/util.h"
#include "evtimer_mbox.h"
#include "netinet/in.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "arpa/inet.h"
#include "sys/socket.h"
#include "unistd.h"
#include "stddef.h"
#include "mbox.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define EMQX_SERVER_IP "47.102.103.1"
#define EMQX_SERVER_PORT 1883
#define TOPIC_NAME "state"
#define MSG_QUEUE_SIZE 8
#define STACK_SIZE 1024

static msg_t msg_queue[MSG_QUEUE_SIZE];
static char stack[STACK_SIZE];

static void publish_state(void) {
    static const char *states[] = {"work", "not work"};
    static uint8_t state_index = 0;

    emcute_topic_t topic;
    topic.name = TOPIC_NAME;
    topic.id = 0; // Topic ID will be assigned by the gateway

    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("Error: could not register topic");
        return;
    }

    const char *state = states[state_index];
    state_index = (state_index + 1) % ARRAY_SIZE(states);

    if (emcute_pub(&topic, state, strlen(state), 0) != EMCUTE_OK) {
        puts("Error: could not publish message");
    } else {
        printf("Published: %s\n", state);
    }
}

static void *mqtt_thread(void *arg) {
    (void)arg;

    sock_udp_ep_t gw = { .family = AF_INET, .port = HTONS(EMQX_SERVER_PORT) };
    if (inet_pton(AF_INET, EMQX_SERVER_IP, &gw.addr.ipv4) <= 0) {
        puts("Error: invalid server address");
        return NULL;
    }

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        puts("Error: could not connect to MQTT-SN gateway");
        return NULL;
    }

    while (1) {
        publish_state();
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "mqtt");

    return 0;
}
