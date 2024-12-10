// Description: 
// This code connects an ESP32 device to an MQTT-SN broker and continuously publishes a state message every five seconds using the emcute library in a RIOT environment. 
// We include the necessary header files in descending alphabetical order as required.

// Include necessary headers in descending alphabetical order by their first letter
#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>

// Define constants
#define MQTT_SN_BROKER_ADDRESS "47.102.103.1"
#define MQTT_SN_BROKER_PORT 1883
#define MQTT_TOPIC "esp32/state"
#define MQTT_STATE_WORK "work"
#define MQTT_STATE_NOT_WORK "not_work"

// Setup thread stack size and priority
char stack[THREAD_STACKSIZE_MAIN];
kernel_pid_t mqtt_thread_pid;

// Function to publish the state
void publish_state(void) {
    emcute_topic_t topic;
    topic.name = MQTT_TOPIC;
    topic.id = 0; // default topic ID

    while (1) {
        // Publish "work" state
        if (emcute_pub(&topic, MQTT_STATE_WORK, strlen(MQTT_STATE_WORK), EMCUTE_QOS_0) != 0) {
            printf("Failed to publish work state\n");
        } else {
            printf("Published work state\n");
        }
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
}

// Main function
int main(void) {
    // Initialize emcute
    emcute_con_t connection;
    netif_t *netif = NULL;

    // Look for the first available netif
    for (unsigned i = 0; i < gnrc_netif_numof(); i++) {
        if (gnrc_netif_get_by_pid(i) != NULL) {
            netif = gnrc_netif_get_by_pid(i);
            break;
        }
    }
    
    if (netif == NULL) {
        printf("No network interface found\n");
        return 1;
    }

    // Connect to the MQTT-SN broker
    if (emcute_con_is_connected(&connection) || emcute_con(&connection, netif, MQTT_SN_BROKER_ADDRESS, MQTT_SN_BROKER_PORT, 0) != 0) {
        printf("Could not connect to MQTT-SN broker at %s\n", MQTT_SN_BROKER_ADDRESS);
        return 1;
    }

    printf("Connected to MQTT-SN broker at %s:%d\n", MQTT_SN_BROKER_ADDRESS, MQTT_SN_BROKER_PORT);

    // Create a thread to publish the state
    mqtt_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST, (thread_func_t)publish_state, NULL, "publish_thread");

    // Keeping the main function alive
    while(1) {
        thread_yield();
    }
    return 0;
}