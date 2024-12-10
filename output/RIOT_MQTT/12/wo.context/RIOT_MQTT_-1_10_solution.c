#include <string.h>  // for string functions
#include <stdio.h>    // for input-output operations like printf
#include <net/ipv6/addr.h>  // for IP address handling
#include <net/sock/udp.h>   // for UDP socket functions
#include <net/sock/tcp.h>   // for TCP socket functions which MQTT may use
#include <MQTTClient.h>  // for MQTT client functions

// MQTT client configuration
#define MQTT_ADDRESS   "47.102.103.1"
#define MQTT_PORT      1883
#define MQTT_TOPIC     "device/state"
#define MQTT_MSG_WORK  "work"
#define MQTT_MSG_NOT_WORK  "not work"

// Timer for 5 seconds
#define TIMER_INTERVAL_SEC   5

static MQTTClient client;
static Network network;
static unsigned char sendbuf[100], readbuf[100];

void publish_state(const char* state) {
    MQTTMessage message;
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)state;
    message.payloadlen = strlen(state);
    MQTTPublish(&client, MQTT_TOPIC, &message);
}

int main(void) {
    NetworkInit(&network);
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    
    int rc = NetworkConnect(&network, MQTT_ADDRESS, MQTT_PORT);
    if (rc != SUCCESS) {
        printf("Failed to connect to network: %d\n", rc);
        return rc;
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "esp32_client";

    rc = MQTTConnect(&client, &data);
    if (rc != SUCCESS) {
        printf("Failed to connect to MQTT broker: %d\n", rc);
        return rc;
    }
    
    printf("Connected to broker successfully!\n");

    while (1) {
        publish_state(MQTT_MSG_WORK);
        printf("Published state: %s\n", MQTT_MSG_WORK);
        sleep(TIMER_INTERVAL_SEC);
    }

    return 0;
}