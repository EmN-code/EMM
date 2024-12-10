#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/emcute.h>
#include <msg.h>
#include <xtimer.h>
#include <net/network_interface.h>
#include <net/sock/udp.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define SLEEP_INTERVAL (5U * US_PER_SEC)

// Function to connect to the MQTT broker without using emcute library
Network connect_to_mqtt_broker(char *address_ip, int port_number) {
    Network network;
    // Dummy function to represent network connection
    // Please insert your real connection handling here
    NetworkConnect(&network, address_ip, port_number);
    return network;
}

// Function to publish a message to the broker
void publish_state(Network *network, const char *topic, const char *message) {
    MQTTClient client;
    MQTTClientInit(&client, network, 1000, NULL, 0, NULL, 0);
    MQTTPublish(&client, topic, (MQTTMessage*)message);
    MQTTDisconnect(&client);
}

int main(void) {
    // Connect to the broker
    Network network = connect_to_mqtt_broker(BROKER_IP, BROKER_PORT);
    
    while (1) {
        // Publish state to the broker
        publish_state(&network, MQTT_TOPIC, MQTT_MESSAGE);

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}