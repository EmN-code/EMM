#include <ztimer.h>  // Include ztimer for timing purposes
#include <net/sock/udp.h>  // Import UDP functionalities for network communication
#include <net/gcoap.h>  // CoAP network functions can be required in some network operations
#include <periph/gpio.h>  // Include GPIO to handle peripheral input/output
#include <paho_mqtt.h>  // Include paho_mqtt for MQTT specific functions
#include <MQTTClient.h>  // Include MQTT client for managing MQTT connections and tasks

#include <stdio.h>      // Standard I/O header

static char sendbuf[128];  // Buffer for sending messages
static char readbuf[128];  // Buffer for reading messages

// Define network parameters
Network network;
MQTTClient client;
MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

// Variable for tracking state
const char* workplace_state = "work";

void my_publish_function(void) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)workplace_state;
    message.payloadlen = strlen(workplace_state);
    
    MQTTPublish(&client, "state", &message);  // Publish the 'state' to the topic
}

int main(void) {
    // Initialize network connection
    NetworkInit(&network);
    NetworkConnect(&network, "47.102.103.1", 1883);
    
    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    // Set up MQTT connection data
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "ESP32Client";
    
    // Connect to the broker
    if (MQTTConnect(&client, &connectData) != SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }
    
    // Setup a ztimer for every five seconds
    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_t timer;
    ztimer_set(clock, &timer, 5000);

    // Loop to periodically publish the state
    while (1) {
        ztimer_sleep(clock, 5000);  // Sleep for 5000ms (5 seconds)
        my_publish_function();  // Publish the state
    }

    return 0;
}