#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

#define BROKER_IP      "47.102.103.1"
#define BROKER_PORT    1883
#define TOPIC_NAME     "device/stat"
#define MQTT_SN_TOPIC  1234  // Arbitrarily chosen topic ID

static emcute_topic_t topic;

static void *publish_thread(void *arg) {
    (void)arg;
    char *status_work = "work";
    char *status_not_work = "not work";
    char state_msg[10];

    while (1) {
        // Toggle state message between "work" and "not work"
        if (strcmp(state_msg, status_work) == 0) {
            strncpy(state_msg, status_not_work, sizeof(state_msg));
        } else {
            strncpy(state_msg, status_work, sizeof(state_msg));
        }

        // Publish the state message
        if (emcute_pub(&topic, state_msg, strlen(state_msg), EMCUTE_QOS_0) != EMCUTE_OK) {
            puts("Error: unable to publish to topic");
        } else {
            printf("Published to topic %s: %s\n", TOPIC_NAME, state_msg);
        }

        // Sleep for five seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    emcute_sub_t subscription;
    emcute_config_t conf;
    const size_t stack_size = THREAD_STACKSIZE_MAIN;
    uint8_t stack[stack_size];
    char *server_addr_str = BROKER_IP;
    printf("Connecting to MQTT-SN broker at %s:%d\n", server_addr_str, BROKER_PORT);

    // Initialize emcute
    memset(&conf, 0, sizeof(conf));
    conf.mqtt_server_ip = server_addr_str;
    conf.mqtt_server_port = BROKER_PORT;
    emcute_init(&conf);

    // Set up the subscription
    subscription.topic.bo.id = MQTT_SN_TOPIC;

    // Manufacturer must assign topic name
    subscription.topic.bo.name = TOPIC_NAME;

    // Allocate stack for publish thread
    thread_create(stack, stack_size, THREAD_PRIORITY_MAIN - 1,
                  0, publish_thread, NULL, "publish_thread");

    return 0;
}