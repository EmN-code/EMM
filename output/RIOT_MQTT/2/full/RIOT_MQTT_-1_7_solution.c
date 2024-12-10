// Description: This program connects to an MQTT broker using RIOT and publishes a message every 5 seconds.

// The program includes the necessary header files in descending alphabetical order by their first letter.

// The main steps include initializing the network, connecting to the MQTT broker, and publishing messages periodically.

// Necessary Includes
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "msg.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "assert.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

// Function to publish a message to the MQTT broker
typedef struct {
    Network n;
    MQTTClient c;
    unsigned char sendbuf[80];
    unsigned char readbuf[80];
} mqtt_client_t;

mqtt_client_t mqtt_client;

void publish_message(MQTTClient* client) {
    MQTTMessage message;
    char payload[] = "State: work or not work";
    message.qos = QOS0;
    message.retained = 0;
    message.dup = 0;
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);

    MQTTPublish(client, "state", &message);
    printf("Published: %s\n", payload);
}

int main(void) {
    // Initialize and connect to the network
    NetworkInit(&mqtt_client.n);
    if (NetworkConnect(&mqtt_client.n, "47.102.103.1", 1883) != 0) {
        printf("Network connection failed\n");
        return 1;
    }

    // Initialize the MQTT client
    MQTTClientInit(&mqtt_client.c, &mqtt_client.n, 1000, mqtt_client.sendbuf, sizeof(mqtt_client.sendbuf), mqtt_client.readbuf, sizeof(mqtt_client.readbuf));

    // Connect to the MQTT broker
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "esp32-client";

    if (MQTTConnect(&mqtt_client.c, &connectData) != 0) {
        printf("MQTT connection failed\n");
        return 1;
    }

    printf("MQTT connected\n");

    // Publish messages every 5 seconds
    while (1) {
        publish_message(&mqtt_client.c);
        xtimer_sleep(5);
    }

    return 0;
}