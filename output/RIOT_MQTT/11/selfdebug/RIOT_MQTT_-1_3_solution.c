#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"
#include "net/ipv4/addr.h"
#include "xtimer.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define CLIENT_ID   "esp32_client"
#define TOPIC       "device/state"
#define INTERVAL    (5U * US_PER_SEC)  // Publish interval in microseconds

static mqtt_client_t client;
static mqtt_sn_pub_msg_t pub_msg;
static char state[12];

// Function to publish a message
static void publish_state(mqtt_client_t *client) {
    snprintf(state, sizeof(state), "work");  // For demonstration, it always publishes 'work'
    pub_msg.topic = TOPIC;
    pub_msg.payload = state;
    pub_msg.payload_len = strlen(state);
    int res = mqtt_sn_send(client, &pub_msg);
    if (res != MQTT_SN_RC_OK) {
        printf("Publish failed with code %d\n", res);
    }
    else {
        printf("Published: %s\n", state);
    }
}

// Main loop
int main(void) {
    mqtt_client_init(&client, CLIENT_ID, 240);  // Initialize client with ID and keepalive timer
    ip_addr_t broker_addr;
    inet_pton(AF_INET, BROKER_IP, &broker_addr);
    int res = mqtt_sn_connect(&client, &broker_addr, BROKER_PORT);
    if (res != MQTT_SN_RC_OK) {
        printf("Failed to connect to broker with code %d\n", res);
        return 1;  // Stop execution if connection is unsuccessful
    }
    printf("Connected to broker at %s:%d\n", BROKER_IP, BROKER_PORT);

    while (1) {
        publish_state(&client);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}