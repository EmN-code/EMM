#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include "net/emcute.h"
#include "net/sock/udp.h"

#define MAIN_QUEUE_SIZE     (4)
#define EMCUTE_PORT         (1883)
#define MQTT_BROKER_IP      "47.102.103.1"
#define TOPIC_NAME          "device/state"
#define DEVICE_ID           "esp32"

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static emcute_topic_t _state_topic;

void *state_pub_thread(void *arg) {
    (void)arg;
    char *state = "work";  // Example state, toggle or modify as needed

    while (1) {
        // Publish state every 5 seconds
        printf("Publishing state: %s\n", state);
        int res = emcute_pub(&_state_topic, state, strlen(state), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("Publication error: %d\n", res);
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    sock_udp_ep_t gw = { .family = AF_INET, .port = EMCUTE_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("Connection to the broker failed\n");
        return 1;
    }

    // Register topic
    _state_topic.name = TOPIC_NAME;
    if (emcute_reg(&_state_topic) != EMCUTE_OK) {
        printf("Registration of topic failed\n");
        return 1;
    }

    // Create a thread to publish state
    char state_pub_stack[THREAD_STACKSIZE_MAIN];
    thread_create(state_pub_stack, sizeof(state_pub_stack), THREAD_PRIORITY_MAIN - 1, 0,
                  state_pub_thread, NULL, "state_pub");

    // Start emcute
    emcute_run(EMCUTE_PORT, DEVICE_ID);

    return 0;
}