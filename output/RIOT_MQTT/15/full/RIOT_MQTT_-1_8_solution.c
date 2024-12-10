/* Description: This program initializes a network and connects an MQTT client to a broker. 
   It periodically publishes a message every 5 seconds.
*/


/* Include necessary header files in descending alphabetical order. */
#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <mqtt_client.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <msg.h>
#include <freertos/task.h>
#include <freertos/FreeRTOS.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stddef.h>
#include <stdatomic.h>
#include <string.h>
#include <sys/socket.h>
#include <architecture.h>

/* Definitions and global variables */
#define MQTT_BROKER_IP        "47.102.103.1"
#define MQTT_BROKER_PORT      1883
#define MQTT_PUBLISH_INTERVAL (5U) /* seconds */
#define MQTT_TOPIC            "state"

static MQTTClient client;
static Network network;

/* Function to publish a message every 5 seconds */
void publish_state(void) {
    int rc;
    MQTTMessage message;
    const char* payload = "work or not work";
    message.payload = (void*)payload;
    message.payloadlen = strlen(payload);
    message.qos = QOS0;
    message.retained = 0;

    while (1) {
        rc = MQTTPublish(&client, MQTT_TOPIC, &message);
        if (rc != 0) {
            printf("Error publishing message: %d\n", rc);
        } else {
            printf("Message published\n");
        }
        ztimer_sleep(ZTIMER_MSEC, MQTT_PUBLISH_INTERVAL * MS_PER_SEC);
    }
}

int main(void) {
    char sendbuf[256], readbuf[256];
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

    int rc;

    printf("Initializing network...\n");

    /* Initialize network */
    memset(&network, 0, sizeof(Network));

    /* Initialize MQTT client */
    MQTTClientInit(&client, &network, 1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    /* Connect to the network */
    while (1) {
        rc = NetworkConnect(&network, MQTT_BROKER_IP, MQTT_BROKER_PORT);
        if (rc != 0) {
            printf("Network connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        } else {
            printf("Connected to network\n");
            break;
        }
    }

    /* Set MQTT connect options */
    data.MQTTVersion = 3;
    data.clientID.cstring = "ESP32_Client";

    /* Connect to the MQTT broker */
    while (1) {
        rc = MQTTConnect(&client, &data);
        if (rc != 0) {
            printf("MQTT connection failed, retrying...\n");
            ztimer_sleep(ZTIMER_USEC, 1000000);
            continue;
        } else {
            printf("Connected to MQTT broker\n");
            break;
        }
    }

    /* Start publishing task */
    publish_state();

    return 0;
}