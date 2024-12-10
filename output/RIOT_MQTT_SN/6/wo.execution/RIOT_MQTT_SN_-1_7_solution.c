#include <ztimer.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <msg.h>
#include <net/emcute.h>
#include <timex.h>
#include <thread.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>

#define _IPV4_ADDR      "47.102.103.1"
#define _MQTT_PORT      1883
#define STATE_TOPIC_NAME "device_state"
#define SLEEP_INTERVAL  (5U * US_PER_SEC)  // sleep for 5 seconds

static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[8];  // define a message queue with 8 messages

static emcute_topic_t state_topic;

static void *publisher_thread(void *arg) {
    (void)arg;  // avoid unused parameter warning
    int pub_flags = EMCUTE_QOS_0;

    while (1) {
        const char *message = "work";  // or "not work", this can be made dynamic
        printf("Publishing state: %s\n", message);

        if (emcute_pub(&state_topic, message, strlen(message), pub_flags) != EMCUTE_OK) {
            printf("[Error] unable to publish data\n");
        }

        // sleep for the defined interval
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    // Initialize the msg queue
    msg_init_queue(queue, 8);

    printf("Connecting to MQTT-SN broker at %s:%d\n", _IPV4_ADDR, _MQTT_PORT);

    sock_udp_ep_t gw = { .family = AF_INET, .port = _MQTT_PORT };
    inet_pton(AF_INET, _IPV4_ADDR, &gw.addr.ipv4);

    if (emcute_con(&gw, true, NULL, NULL, 0, 0) != EMCUTE_OK) {
        printf("[Error] unable to connect to the gateway\n");
        return 1;
    }

    // Set up the topic for publishing state
    state_topic.name = STATE_TOPIC_NAME;
    if (emcute_reg(&state_topic) != EMCUTE_OK) {
        printf("[Error] unable to obtain topic ID\n");
        return 1;
    }

    // Start the publisher thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publisher_thread, NULL, "publisher");

    return 0;
}