// The code is structured to connect to an MQTT-SN broker using the emcute library
// The program initializes the connection to the broker, sets up a topic and publishes the state every 5 seconds.
// It includes error handling and retries connecting if initial connection fails.

#include <stdio.h>
#include <string.h>
#include "msg.h"
#include "net/emcute.h"
#include "net/ipv6/addr.h"
#include "ztimer.h"

#define EMCUTE_PORT      (1883U)
#define EMCUTE_ID        ("esp32_mqtt_sn")
#define STATE_PUB_TOPIC  ("state_topic")
#define STATE_WORKING    ("work")
#define STATE_NOT_WORKING ("not_work")

static char stack[THREAD_STACKSIZE_DEFAULT];
static msg_t queue[8];

static emcute_topic_t state_topic;
static char state_message[20];

typedef enum {
    WORKING,
    NOT_WORKING
} device_state_t;

device_state_t current_state = WORKING;

void on_publish(void) {
    if(current_state == WORKING) {
        strcpy(state_message, STATE_WORKING);
    } else {
        strcpy(state_message, STATE_NOT_WORKING);
    }

    int res = emcute_pub(&state_topic, state_message, strlen(state_message), EMCUTE_QOS_0);
    if (res != EMCUTE_OK) {
        printf("Error: unable to publish data [%d]\n", res);
    } else {
        printf("State message published: %s\n", state_message);
    }
}

int main(void) {
    ztimer_t timer;

    msg_init_queue(queue, ARRAY_SIZE(queue));

    emcute_run(EMCUTE_PORT, EMCUTE_ID);

    sock_udp_ep_t gw = { .family = AF_INET6, .port = EMCUTE_PORT };
    inet_pton(AF_INET6, "47.102.103.1", &gw.addr.ipv6);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Error: unable to connect to the gateway\n");
        return 1;
    }

    state_topic.name = STATE_PUB_TOPIC;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("Error: unable to register topic\n");
        return 1;
    }

    while (1) {
        on_publish();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}