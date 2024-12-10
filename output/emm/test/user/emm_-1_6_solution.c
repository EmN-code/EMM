#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "dht.h"

#define MQTT_PORT (1883U)
#define DEFAULT_MQTT_BROKER_ADDR "47.102.103.1"
#define DEFAULT_MQTT_TOPIC "sensor/data"
#define DHT_PIN GPIO_PIN(0, 0) // Adjust for the correct pin
#define SLEEP_TIME (60U)

static dht_t dht;
static MQTTAsync client;

// MQTT connection lost callback
void on_connection_lost(void *context, char *cause) {
    printf("Connection lost: %s\n", cause);
}

// MQTT message arrived callback
int message_arrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message) {
    // Handle messages here if subscribed to topics
    MQTTAsync_freeMessage(&message);
    MQTTAsync_free(topicName);
    return 1;
}

// MQTT connected
void on_connect(void *context, MQTTAsync_successData *response) {
    printf("Connected to the MQTT broker!\n");
}

// MQTT connection failed
void on_connect_failure(void *context, MQTTAsync_failureData *response) {
    printf("Failed to connect, rc %d\n", response ? response->code : 0);
}

// Connect to MQTT Broker using PAHO MQTT
static int connect_to_broker() {
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;
    if ((rc = MQTTAsync_create(&client, DEFAULT_MQTT_BROKER_ADDR, "DHT11Client",
                              MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS) {
        printf("Failed to create client, return code %d\n", rc);
        return -1;
    }

    if ((rc = MQTTAsync_setCallbacks(client, NULL, on_connection_lost, message_arrived, NULL)) != MQTTASYNC_SUCCESS) {
        printf("Failed to set callbacks, return code %d\n", rc);
        return -1;
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.onSuccess = on_connect;
    conn_opts.onFailure = on_connect_failure;

    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to start connect, return code %d\n", rc);
        return -1;
    }

    return 0; // Success
}

// Publish Data over MQTT
static void publish_data(int16_t temp, int16_t hum) {
    char payload[64];
    snprintf(payload, sizeof(payload), "Temperature: %d, Humidity: %d", temp, hum);
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
    int rc;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(payload);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    opts.onSuccess = NULL;
    opts.onFailure = NULL;
    opts.context = client;

    if ((rc = MQTTAsync_sendMessage(client, DEFAULT_MQTT_TOPIC, &pubmsg, &opts)) != MQTTASYNC_SUCCESS) {
        printf("Failed to send message, return code %d\n", rc);
    } else {
        printf("Published message to topic %s: %s\n", DEFAULT_MQTT_TOPIC, payload);
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

    // Connect to MQTT broker
    if (connect_to_broker() != 0) {
        return 1;
    }

    // Main loop
    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            publish_data(temp, hum);
        } else {
            puts("Failed to read from DHT sensor");
        }

        xtimer_sleep(SLEEP_TIME);
    }

    MQTTAsync_destroy(&client);

    return 0;
}