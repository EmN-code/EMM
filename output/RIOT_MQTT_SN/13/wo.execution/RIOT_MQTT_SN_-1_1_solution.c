#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "kernel.h"
#include "msg.h"
#include "net/emcute.h"

#define MQTT_TOPIC     "state"
#define MQTT_PORT      1883
#define DEVICE_ID      "ESP32_device"
#define BROKER_IP      "47.102.103.1"

#define RECONNECT_INTERVAL (5U * US_PER_SEC)  // 5 seconds in microseconds

static emcute_sock_t sock;
static char stack[THREAD_STACKSIZE_MAIN];
static msg_t queue[4];  // Message queue size

void *mqtt_publisher(void *arg) {
    (void)arg;
    emcute_topic_t t;
    unsigned flags = EMCUTE_QOS_0;
    const char *data = "work";

    while (1) {
        if (emcute_con(&sock, BROKER_IP, MQTT_PORT, DEVICE_ID) != EMCUTE_OK) {
            printf("Connection failed, retrying...\n");
            xtimer_sleep(RECONNECT_INTERVAL / (US_PER_SEC));
            continue;
        }

        t.name = MQTT_TOPIC;
        if (emcute_reg(&t) != EMCUTE_OK) {
            printf("Registration failed\n");
            continue;
        }

        while (true) {
            if (emcute_pub(&t, data, strlen(data), flags) != EMCUTE_OK) {
                printf("Error: unable to publish data\n");
            } else {
                printf("Data published: %s\n", data);
            }
            xtimer_sleep(RECONNECT_INTERVAL / (US_PER_SEC));   
        }
    }   
    return NULL;
}

int main(void) {
    msg_init_queue(queue, 4);
    // create thread
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0,
                                     mqtt_publisher, NULL, "publisher");
    while (1) {
        // main thread can perform other tasks
        xtimer_sleep(10);
    }
    return 0;
}