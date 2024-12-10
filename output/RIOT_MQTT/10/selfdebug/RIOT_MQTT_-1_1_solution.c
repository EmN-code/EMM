#include <stdio.h>  
#include "xtimer.h" 
#include "net/emcute.h" 
#include "net/sock/udp.h"  
#include "net/sock.h" 
#include "msg.h"
#include "net/mqttsn.h" 
#include "simple_udp.h"

#define MQTT_BROKER_HOST    "47.102.103.1"
#define MQTT_BROKER_PORT    1883
#define CLIENT_ID           "riot_esp32_mqtt_client"
#define TOPIC               "device/state"

static char line_buf[1024]; // Buffer for MQTT messages
static unsigned char send_buf[1024];
static unsigned char receive_buf[1024];

int main(void) {
    Network network;
    MQTTClient client;
    struct sockaddr_in addr;
    MQTTMessage message;
    int rc = 0;

    printf("[MQTT] Initializing MQTT Client...\n");
    MQTTClientInit(&client, &network, 1000, send_buf, sizeof(send_buf), receive_buf, sizeof(receive_buf));
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MQTT_BROKER_PORT);
    addr.sin_addr.s_addr = inet_addr(MQTT_BROKER_HOST);
    
    printf("[MQTT] Connecting to broker at %s:%d...\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    if ((rc = NetworkConnect(&network, MQTT_BROKER_HOST, MQTT_BROKER_PORT)) != 0) {
        printf("[ERROR] Failed to connect to network with error %d\n", rc);
        return -1;
    }
    
    MQTTStartTask(&client);

    while (1) {
        message.qos = 1;
        message.retained = 0;
        message.dup = 0;
        message.payload = (void*)"work";
        message.payloadlen = strlen("work");

        printf("[MQTT] Publishing message...\n");
        if ((rc = MQTTPublish(&client, TOPIC, &message)) != 0) {
            printf("[ERROR] Failed to publish message with error %d\n", rc);
        } else {
            printf("[INFO] Message published successfully\n");
        }

        xtimer_sleep(5); // Wait for 5 seconds before publishing again
    }

    return 0;
}