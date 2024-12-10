#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_device"
#define MQTT_USERNAME "your_username"
#define MQTT_PASSWORD "your_password"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 1
#define MQTT_PUBLISH_INTERVAL (5 * 1000 * 1000) // 5 seconds in microseconds

// MQTT client structure
static mqtt_client_t client;

// Function to handle MQTT events
static void _on_mqtt_event(mqtt_client_t *client, void *param, mqtt_event_t event, void *data) {
    switch (event) {
        case MQTT_EVENT_CONNECT:
            printf("Connected to MQTT broker\n");
            break;
        case MQTT_EVENT_DISCONNECT:
            printf("Disconnected from MQTT broker\n");
            break;
        case MQTT_EVENT_SUBACK:
            printf("Subscription acknowledged\n");
            break;
        case MQTT_EVENT_PUBACK:
            printf("Publication acknowledged\n");
            break;
        default:
            break;
    }
}

// Function to publish state
static void _publish_state(void) {
    char state[] = "work";
    mqtt_message_t message = {
        .topic = MQTT_TOPIC,
        .payload = state,
        .payload_len = strlen(state),
        .qos = MQTT_QOS,
        .retain = 0
    };

    if (mqtt_publish(&client, &message) == MQTT_OK) {
        printf("Published state: %s\n", state);
    } else {
        printf("Failed to publish state\n");
    }
}

// Main thread function
void *mqtt_thread(void *arg) {
    (void)arg;

    // Initialize the MQTT client
    mqtt_init(&client, NULL, _on_mqtt_event, NULL, MQTT_BROKER_IP, MQTT_BROKER_PORT, MQTT_KEEPALIVE);

    // Connect to the MQTT broker
    mqtt_connect(&client, MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD, 0, 0, 0, 0);

    // Wait for the connection to be established
    while (!mqtt_is_connected(&client)) {
        xtimer_sleep(1);
    }

    // Publish state every 5 seconds
    while (1) {
        _publish_state();
        xtimer_usleep(MQTT_PUBLISH_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Create the MQTT thread
    kernel_pid_t mqtt_pid = thread_create("mqtt_thread", THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, mqtt_thread, NULL, "MQTT Thread");

    // Start the RIOT scheduler
    msg_loop();

    return 0;
}