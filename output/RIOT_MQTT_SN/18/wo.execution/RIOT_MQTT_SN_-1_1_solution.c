#include "xtimer.h"
#include "thread.h"
#include "net/emcute.h"

#define MQTT_BROKER_ADDR     "47.102.103.1"
#define MQTT_BROKER_PORT     1883
#define PUBLISH_INTERVAL     (5U * 1000U * 1000U) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];

// Function to publish a message to the MQTT-SN broker
static void *mqtt_publish(void *arg) {
    (void)arg;
    emcute_topic_t topic;
    topic.name = "device/state";
    topic.id = 0;

    while (1) {
        const char *state = "work";
        emcute_publish(&topic, state, strlen(state) + 1, EMCUTE_QOS_0);
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return NULL;
}

// Main function
int main(void) {
    // Initialize the emcute client
    emcute_config_t config;
    config.port = MQTT_BROKER_PORT;
    config.addr = addr;
    emcute_init(&config);

    // Create a thread for publishing messages
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_publish, NULL, "mqtt_publish_thread");

    // Keeping the main function running
    while (1) {
        xtimer_sleep(1);
    }
 
    return 0;
}