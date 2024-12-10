#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "MQTTClient.h"

// Define constants for broker connection
#define MQTT_BROKER_IP   "192.168.1.100"
#define MQTT_BROKER_PORT 1883

// Function prototypes
void my_timer_callback(void *arg);

int main(void) {
    Network network;
    int rc;

    // Attempt to connect to MQTT broker
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("connect failed\n");
            // Sleep for 1 second before trying again
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("connect successfully\n");
            break;
        }
    }

    // Additional logic here to handle MQTT client creation and subscription
    // Setup a timer here to call my_timer_callback function every second

    return 0;
}

void my_timer_callback(void *arg) {
    // Logic to turn on the LED
    printf("LED turned on\n");
}