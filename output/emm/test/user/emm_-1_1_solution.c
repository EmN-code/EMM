#include <xtimer.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "paho_mqtt.h"
#include "net/sock/udp.h"
#include "msg.h"
#include "dht.h"

#define MQTT_PORT (1883U)
#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_TOPIC "sensor/data"
#define DHT_PIN GPIO_PIN(0, 0) // Adjust to the correct pin
#define SLEEP_INTERVAL (60U)

static dht_t dht;

// MQTT connection callback function
static void on_connect(void *arg, MQTTAsync_successData *response) {
    (void)response;
    printf("MQTT connected\n");
}

// MQTT connection lost callback
static void on_connection_lost(void *arg, char *cause) {
    printf("MQTT connection lost: %s\n", cause);
}

// Publish temperature and humidity data
static void publish_data(MQTTAsync client, int16_t temp, int16_t hum) {
    char payload[64];
    snprintf(payload, sizeof(payload), "Temperature: %d, Humidity: %d", temp, hum);

    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;
    if ((rc = MQTTAsync_sendMessage(client, MQTT_TOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to publish message, return code %d\n", rc);
    } else {
        printf("Published: %s\n", payload);
    }
}

int main(void) {
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Initialize DHT sensor
    dht.params.pin = DHT_PIN;
    if (dht_init(&dht, &dht.params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // MQTT client configuration
    MQTTAsync client;
    MQTTAsync_create(&client, MQTT_BROKER_IP, "RIOT_Client", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTAsync_setCallbacks(client, NULL, on_connection_lost, NULL, NULL);

    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    conn_opts.onSuccess = on_connect;
    conn_opts.onFailure = NULL;
    conn_opts.cleansession = 1;

    // Connect to MQTT Broker
    if (MQTTAsync_connect(client, &conn_opts) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect to MQTT broker\n");
        return 1;
    }

    // Main loop
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            publish_data(client, temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    // Clean up MQTT client
    MQTTAsync_destroy(&client);

    return 0;
}