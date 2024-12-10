#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/sock/udp.h"
#include "net/sock/tcp.h"
#include "net/mqtt.h"
#include "periph/gpio.h"

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define MESSAGE "work"
#define MESSAGE_NOT_WORK "not work"
#define INTERVAL 5000000 // 5 seconds in microseconds

// Function to initialize the network
static void network_init(void) {
    // Initialize the network stack
    // This is typically done by the RIOT OS, so we don't need to do anything here
}

// Function to connect to the MQTT broker
static int mqtt_connect(mqtt_client_t *client, const char *broker_ip, uint16_t broker_port) {
    // Create a network connection
    sock_tcp_t sock;
    sock_tcp_init(&sock, NULL, NULL);

    // Connect to the broker
    if (sock_tcp_connect(&sock, broker_ip, broker_port, SOCK_ADDR_ANY_NETIF) < 0) {
        printf("Failed to connect to broker\n");
        return -1;
    }

    // Initialize the MQTT client
    mqtt_client_init(client, &sock, 5000, NULL, 0, NULL, 0);

    // Connect to the MQTT broker
    mqtt_connmsg_t connmsg = {
        .keep_alive = 60,
        .clean_session = true,
        .will_retain = false,
        .will_qos = MQTT_QOS_0,
        .will_topic = NULL,
        .will_message = NULL,
        .username = NULL,
        .password = NULL
    };

    if (mqtt_connect_client(client, &connmsg) < 0) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    printf("Connected to MQTT broker\n");
    return 0;
}

// Function to publish a message
static int mqtt_publish(mqtt_client_t *client, const char *topic, const char *message) {
    mqtt_topic_t topics[] = {
        { .topic = topic, .qos = MQTT_QOS_0 }
    };

    mqtt_message_t msg = {
        .message = message,
        .len = strlen(message),
        .retain = false,
        .dup = false,
        .qos = MQTT_QOS_0
    };

    if (mqtt_publish(client, topics, 1, &msg) < 0) {
        printf("Failed to publish message\n");
        return -1;
    }

    printf("Published message: %s\n", message);
    return 0;
}

// Main function
int main(void) {
    // Initialize the network
    network_init();

    // Initialize the MQTT client
    mqtt_client_t client;

    // Connect to the MQTT broker
    if (mqtt_connect(&client, BROKER_IP, BROKER_PORT) < 0) {
        return -1;
    }

    // Main loop
    while (1) {
        // Publish the state
        if (mqtt_publish(&client, TOPIC, MESSAGE) < 0) {
            printf("Failed to publish message\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / US_PER_SEC);

        // Publish the not working state
        if (mqtt_publish(&client, TOPIC, MESSAGE_NOT_WORK) < 0) {
            printf("Failed to publish message\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}