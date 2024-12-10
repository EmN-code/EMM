#include "ztimer.h"
#include "paho_mqtt.h"
#include <string.h>
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "xtimer.h"
#include <stdio.h>
#include <sys/socket.h>
#include "msg.h"
#include "net/sock/udp.h"
#include "dht.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include <stdlib.h>

#define MQTT_PORT (1883U)
#define DEFAULT_MQTT_BROKER_ADDR "47.102.103.1"
#define DEFAULT_MQTT_TOPIC "sensor/data"
#define DHT_PIN GPIO_PIN(0, 0) // Adjust for the correct pin
#define SLEEP_TIME (60U)

static dht_t dht;

static MQTTClient mqtt_client;
static void on_connect(void* context, MQTTAsync_successData* response) {
    printf("Connected to MQTT broker\n");
}

static int connect_to_broker(void) {
    char address[256];
    snprintf(address, sizeof(address), "tcp://%s:%d", DEFAULT_MQTT_BROKER_ADDR, MQTT_PORT);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&mqtt_client, address, "RIOT_Client", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(mqtt_client, NULL, NULL, NULL, NULL);
    conn_opts.cleansession = 1;

    if (MQTTClient_connect(mqtt_client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return -1;
    }

    return 0;
}

static void publish_data(int16_t temp, int16_t hum) {
    char message[64];
    snprintf(message, sizeof(message), "Temperature: %d, Humidity: %d", temp, hum);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = message;
    pubmsg.payloadlen = (int)strlen(message);
    pubmsg.qos = 1;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(mqtt_client, DEFAULT_MQTT_TOPIC, &pubmsg, NULL);
    printf("Published message to topic %s: %s\n", DEFAULT_MQTT_TOPIC, message);
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

        ztimer_sleep(ZTIMER_MSEC, SLEEP_TIME * 1000); // Sleep for SLEEP_TIME seconds
    }

    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);

    return 0;
}