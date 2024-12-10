#include "net/asymcute.h"
#include <stdio.h>
#include <xtimer.h>

#define MQTT_SN_BROKER_IP "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_TOPIC_ID 1
#define PUB_INTERVAL 5 /* in seconds */

static char stack[THREAD_STACKSIZE_MAIN];
static emcute_sub_t subscription;
static emcute_topic_t topic;

static const char *state_message = "work";

static void publish_state(void) {
    asymcute_con_t *connection = NULL; // Initialize connection to NULL
    connection = emcute_get_connection(); // Get the emcute connection
    
    if (!connection) {
        printf("Error connecting to MQTT-SN broker\n");
        return;
    }

    topic.id = MQTT_TOPIC_ID;
    topic.name = "state";
    
    // Publish data
    asymcute_req_t pub_req;
    pub_req.flags = EMCUTE_QOS_0; // Set QoS level 0
    int res = emcute_publish(connection, &pub_req, &topic, state_message, strlen(state_message));
    if (res != EMCUTE_OK) {
        printf("Error publishing data\n");
    } else {
        printf("Data published: %s\n", state_message);
    }

    xtimer_sleep(PUB_INTERVAL); // Wait for the next publish interval
}

void *publish_thread(void *arg) {
    while(1) {
        publish_state();
    }
    return NULL;
}

int main(void) {
    printf("MQTT-SN Publish Example\n");

    // Start publish thread
    kernel_pid_t publish_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, publish_thread, NULL, "publish");

    if (publish_pid <= KERNEL_PID_UNDEF) {
        printf("Failed to start publish thread\n");
    } else {
        printf("Publish thread started\n");
    }

    // Keep main thread busy
    while (1) {
        xtimer_sleep(PUB_INTERVAL);
    }

    return 0;
}