#include <string.h>  // Include necessary standard library headers for string functions
#include <stdlib.h>  // Standard library for memory allocation, process control, etc.
#include <stdio.h>  // Standard input and output definitions
#include <mqtt.h>  // Include MQTT specific functionalities from the RIOT environment 
#include <net/netdev.h>  // Networking device interface
#include <net/sock/tcp.h>  // Include Packet buffer interface for TCP
#include <net/ipv6/addr.h>  // IPv6 address related functionalities
#include <periph/rtc.h>  // Real Time Clock peripheral interface
#include <xtimer.h>  // Timer library for RIOT OS

#define BROKER_IP "47.102.103.1"  // Define the MQTT broker IP address
#define BROKER_PORT 1883  // Define the MQTT broker port number
#define CLIENT_ID   "esp32_riot_mqtt"  // Define the MQTT client ID
#define KEEPALIVE_INTERVAL 10  // Set keep-alive interval for MQTT broker

static mqtt_client_t client;  // Initialize a client specific structure for MQTT
static char status[12];  // Array to hold the status message

/* Function to handle message publishing */
void publish_status(void)
{
    strcpy(status, "work");  // Copy the status message to the buffer
    msg_t msg;
    msg.type = 0;
    msg.content.value = status;
    int res = mqtt_publish(&client, msg, NULL);
    printf(res == 0 ? "Published: %s\n" : "Error publishing: %d\n", status, res);
}

/* MQTT event handler function */
static void on_connected(mqtt_client_t *client, int status)
{
    if (status != MQTT_CONNECTED) {
        puts("MQTT connection failed");
        return;
    }
    puts("MQTT connected successfully");
}

/* Main function implementation */
int main(void)
{
    /* Set up MQTT client configuration */
    mqtt_client_config_t config;
    memset(&config, 0, sizeof(config));  // Initialize the MQTT client config to zero

    config.broker_url = BROKER_IP;  // Assign the defined Broker IP
    config.broker_port = BROKER_PORT;  // Assign the defined Broker Port
    config.client_id = CLIENT_ID;  // Assign the defined Client ID
    config.keepalive = KEEPALIVE_INTERVAL;  // Set the keep-alive interval
    config.connected_cb = on_connected; // Set callback function for connection

    int res = mqtt_connect(&client, &config);
    if (res != MQTT_CONNECTED) {
        printf("MQTT connection failed with status %d\n", res);
        return 1;
    }

    puts("MQTT client connecting...");

    /* Periodically publish status every 5 seconds */
    while (1) {
        publish_status();
        xtimer_sleep(5);
    }

    return 0;
}
