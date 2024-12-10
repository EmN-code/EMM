#include <xtimer.h> // Timer related functions.
#include <stdio.h>   // Standard input and output
#include <mqtt_client.h>  // MQTT Client handle and functions
#include <esp_wifi.h> // ESP WiFi initialization
#include <MQTTClient.h> // MQTT client definitions

// Define parameters for the MQTT connection
#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define MQTT_TOPIC "state/topic"
#define CLIENT_ID "esp32_client"
#define QOS 1 // Quality of Service

// Define the interval to publish the message (5 seconds)
#define PUBLISH_INTERVAL (5 * 1000000U) // Convert to microseconds

static MQTTClient client;
static char message_data[50];
static Network network;

void publish_callback(void *arg){
    (void)arg;
    // Create the state message
    MQTTMessage message;
    message.qos = QOS;
    message.retained = 0;
    message.dup = 0;
    message.payload = (char*)message_data;
    message.payloadlen = snprintf(message_data, sizeof(message_data), "work or not work");
    
    // Publish the state to the defined topic
    MQTTPublish(&client, MQTT_TOPIC, &message);
    printf("Published: %s\n", message_data);
}

void mqtt_setup(void){
    int rc;
    unsigned char sendbuf[80], readbuf[80];
    
    // Initialize the network structure
    NetworkInit(&network);
    rc = NetworkConnect(&network, BROKER_IP, BROKER_PORT);
    if (rc != 0) {
        printf("Error connecting to the network\n");
        return;
    }

    // Initialize the MQTT client
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Create and prepare the connection data
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = 20;
    data.cleansession = 1;
    data.willFlag = 0;
    
    // Connect the MQTT client to the broker
    rc = MQTTConnect(&client, &data);
    if (rc != 0) {
        printf("Error connecting to the MQTT broker\n");
        return;
    }

    printf("MQTT connection successful\n");
}

int main(void){
    // Setup network and MQTT
    esp_wifi_init(NULL);
    mqtt_setup();
    
    // Timer for periodic publishing
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while(1) {
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
        publish_callback(NULL);
    }

    return 0;
}