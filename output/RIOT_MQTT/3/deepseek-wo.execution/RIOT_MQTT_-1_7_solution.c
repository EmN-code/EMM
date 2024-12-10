
#include <net/sock.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include <stdatomic.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <timex.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <dht.h>
#include <ztimer.h>
#include <stddef.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 1
#define KEEP_ALIVE 20

static MQTTClient client;
static Network network;

void mqtt_message_arrived(MessageData* data) {
    (void)data;
}

void mqtt_delivery_complete(void* token) {
    (void)token;
}

void mqtt_connection_lost(void* context) {
    (void)context;
}

int main(void) {
    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    int rc;

    // Initialize network
    NetworkInit(&network);

    // Create MQTT client
    MQTTClientInit(&client, &network, 1000, NULL, 0, NULL, 0);

    // Set connection options
    options.keepAliveInterval = KEEP_ALIVE;
    options.cleansession = 1;
    options.MQTTVersion = 3;

    // Connect to MQTT broker
    rc = MQTTConnect(&client, &options);
    if (rc != 0) {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        return -1;
    }

    // Set MQTT callbacks
    MQTTClient_setCallbacks(&client, NULL, mqtt_connection_lost, mqtt_message_arrived, mqtt_delivery_complete);

    // Publish message every 5 seconds
    while (1) {
        MQTTMessage message;
        message.qos = QOS;
        message.retained = 0;
        message.payload = (void*)PAYLOAD;
        message.payloadlen = strlen(PAYLOAD);

        rc = MQTTPublish(&client, TOPIC, &message);
        if (rc != 0) {
            printf("Failed to publish message, return code %d\n", rc);
        }

        xtimer_sleep(5);
    }

    // Disconnect and clean up
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);

    return 0;
}
