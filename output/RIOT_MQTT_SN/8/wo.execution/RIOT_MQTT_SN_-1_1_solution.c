#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <msg.h>
#include <net/emcute.h>
#include <xtimer.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define PUB_INTERVAL (5U * 1000U * 1000U) // publish interval in microseconds (5 seconds)
#define TOPIC_NAME "state"
#define TOPIC_MSG "work or not work"

static emcute_broker_t broker;
static emcute_topic_t topic;

static void *mqtt_thread(void *arg) {
    (void)arg;
    while (1) {
        xtimer_usleep(PUB_INTERVAL);  // sleep for the specified interval

        int res = emcute_pub(&topic, TOPIC_MSG, strlen(TOPIC_MSG), EMCUTE_QOS_0);
        if (res != EMCUTE_OK) {
            printf("[ERROR] Unable to publish data to %s
", TOPIC_NAME);
        } else {
            printf("Published data: %s
", TOPIC_MSG);
        }
    }
    return NULL;
}

int main(void) {
    puts("MQTT-SN Publisher Application");

    /* Network initialization here, e.g., setting up the network interface */

    /* Initialize broker data */
    memset(&broker, 0, sizeof(broker));
    broker.port = BROKER_PORT;
    broker.address.family = AF_INET;
    if (inet_pton(AF_INET, BROKER_IP, &broker.address.addr) <= 0) {
        puts("[ERROR] Malformed broker IP address");
        return 1;
    }

    /* Connect to broker */
    if (emcute_con(&broker, true) != EMCUTE_OK) {
        puts("[ERROR] Could not connect to broker
");
        return 1;
    }
    puts("Successfully connected to broker
");

    /* Set up a topic */
    topic.name = TOPIC_NAME;
    if (emcute_reg(&topic) != EMCUTE_OK) {
        puts("[ERROR] Unable to register topic
");
        return 1;
    }
    printf("Registered topic: %s
", topic.name);

    /* Start the MQTT thread */
    char priority_stack[THREAD_STACKSIZE_MAIN];
    thread_create(priority_stack, sizeof(priority_stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_thread, NULL, "mqtt_publish_thread");

    return 0;
}