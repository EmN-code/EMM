#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "mqtt_client.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE_WORK "work"
#define MQTT_MESSAGE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

static char stack[THREAD_STACKSIZE_DEFAULT];
static mqtt_client_t client;

static void _on_puback(mqtt_client_t *client, void *param, uint16_t msg_id) {
    (void)client;
    (void)param;
    (void)msg_id;
    printf("PUBACK received\n");
}

static void _on_connect(mqtt_client_t *client, void *param, mqtt_connection_state_t state) {
    (void)client;
    (void)param;
    if (state == MQTT_CS_CONNECTED) {
        printf("Connected to MQTT broker\n");
    } else {
        printf("Failed to connect to MQTT broker\n");
    }
}

static void _publish_state(void) {
    static bool is_working = true;
    const char *message = is_working ? MQTT_MESSAGE_WORK : MQTT_MESSAGE_NOT_WORK;
    is_working = !is_working; // Toggle state

    mqtt_message_t msg = {
        .qos = MQTT_QOS_0,
        .retain = 0,
        .topic = MQTT_TOPIC,
        .payload = message,
        .payload_len = strlen(message),
    };

    if (mqtt_publish(&client, &msg, _on_puback, NULL) == MQTT_OK) {
        printf("Published message: %s\n", message);
    } else {
        printf("Failed to publish message\n");
    }
}

static void *mqtt_task(void *arg) {
    (void)arg;

    // Initialize the MQTT client
    mqtt_client_init(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT, NULL, NULL, NULL, 0);

    // Set the connection callback
    mqtt_set_connect_handler(&client, _on_connect, NULL);

    // Connect to the MQTT broker
    if (mqtt_connect(&client) != MQTT_OK) {
        printf("Failed to connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        // Yield to allow other tasks to run
        xtimer_sleep(1);

        // Check if connected
        if (mqtt_is_connected(&client)) {
            // Publish the state
            _publish_state();

            // Wait for the next interval
            xtimer_usleep(PUBLISH_INTERVAL);
        } else {
            // Reconnect if disconnected
            if (mqtt_connect(&client) == MQTT_OK) {
                printf("Reconnected to MQTT broker\n");
            } else {
                printf("Failed to reconnect to MQTT broker\n");
            }
        }
    }

    return NULL;
}

int main(void) {
    // Create the MQTT task
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, mqtt_task, NULL, "mqtt_task");

    // Main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}