
#include "thread.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "event/thread.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MQTT_BROKER_IP  "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC_NAME  "state"
#define MQTT_MESSAGE "work or not work"
#define PUB_INTERVAL 5

static char stack[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread_pid;

void *mqtt_pub_thread(void *arg) {
    (void)arg;

    emcute_topic_t topic;
    topic.name = MQTT_TOPIC_NAME;

    while (1) {
        int rc;

        rc = emcute_pub(&topic, MQTT_MESSAGE, strlen(MQTT_MESSAGE), EMCUTE_QOS_0);
        if (rc != EMCUTE_OK) {
            printf("Publish failed: %d\n", rc);
            continue;
        }

        printf("Published: %s to topic %s\n", MQTT_MESSAGE, MQTT_TOPIC_NAME);

        for (int i = 0; i < PUB_INTERVAL; ++i) {
            if (thread_pid == KERNEL_PID_UNDEF) {
                break;
            }

            thread_sleep(1);
        }
    }

    return NULL;
}

int main(void) {
    sock_udp_ep_t gw = { .family = AF_INET6, .port = MQTT_BROKER_PORT };

    while (1) {
        int rc;

        rc = emcute_con(&gw, true, NULL, NULL, 0, 0);
        if (rc != EMCUTE_OK) {
            printf("Connection failed: retrying in 1s\n");
            sys_sleep(1);
            continue;
        }

        printf("Connected to MQTT-SN broker\n");
        break;
    }

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_pub_thread, NULL, "mqtt_pub_thread");

    return 0;
}
