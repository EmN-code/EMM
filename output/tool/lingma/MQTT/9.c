#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "device/state"
#define STATE_WORK "work"
#define STATE_NOT_WORK "not work"
#define PUBLISH_INTERVAL (5 * 1000000) // 5 seconds in microseconds

// MQTT client structure
static mqtt_client_t client;

// Network structure
static struct {
    sock_tcp_t sock;
    int (*mqttread)(struct Network*, unsigned char*, int, int);
    int (*mqttwrite)(struct Network*, unsigned char*, int, int);
} network;

// Function to initialize the network connection
static void network_init(void) {
    network.mqttread = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_read;
    network.mqttwrite = (int (*)(struct Network*, unsigned char*, int, int))sock_tcp_write;
}

// Function to connect to the MQTT broker
static int connect_to_broker(void) {
    sock_tcp_t sock;
    sock_tcp_init(&sock, NULL);

    // Connect to the broker
    if (sock_tcp_connect(&sock, BROKER_IP, BROKER_PORT, NULL) < 0) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    network.sock = sock;
    return 0;
}

// Function to publish a message
static int publish_message(const char *message) {
    mqtt_message_t msg;
    msg.qos = MQTT_QOS0;
    msg.retained = 0;
    msg.dup = 0;
    msg.id = 0;
    msg.payload = (void *)message;
    msg.payloadlen = strlen(message);

    return mqtt_publish(&client, TOPIC, &msg);
}

// Main function
int main(void) {
    // Initialize the network
    network_init();

    // Connect to the broker
    if (connect_to_broker() < 0) {
        return -1;
    }

    // Initialize the MQTT client
    mqtt_client_init(&client, &network, 10000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    mqtt_connmsg_t connmsg = {
        .keep_alive = 60,
        .clean_session = 1,
        .will_retain = 0,
        .will_qos = MQTT_QOS0,
        .will_topic = NULL,
        .will_message = NULL,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect(&client, &connmsg) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    // Main loop
    while (1) {
        // Publish the state
        if (publish_message(STATE_WORK) < 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", STATE_WORK);
        }

        // Wait for 5 seconds
        xtimer_sleep(PUBLISH_INTERVAL / 1000000);

        // Publish the state
        if (publish_message(STATE_NOT_WORK) < 0) {
            printf("Failed to publish message\n");
        } else {
            printf("Published message: %s\n", STATE_NOT_WORK);
        }

        // Wait for 5 seconds
        xtimer_sleep(PUBLISH_INTERVAL / 1000000);
    }

    return 0;
}