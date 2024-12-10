#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "sys/socket.h"
#include "string.h"
#include "msg.h"
#include "net/sock/udp.h"
#include "dht.h"
#include "evtimer_msg.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "thread.h"
#include "event/thread.h"
#include "net/emcute.h"
#include "stdlib.h"

#define MQTT_PORT (1883U)
#define DEFAULT_MQTT_BROKER_ADDR "tcp://127.0.0.1:1883"
#define DEFAULT_MQTT_TOPIC "sensor/data"
#define DHT_PIN GPIO_PIN(0, 0) // Adjust for the correct pin
#define SLEEP_TIME (60U)

static dht_t dht;

void deliveryComplete(void *context, MQTTClient_deliveryToken dt) {
    printf("Delivery Complete for token: %d\n", dt);
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

    // Initialize and connect MQTT client
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, DEFAULT_MQTT_BROKER_ADDR, "ExampleClientID",
                      MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, NULL, deliveryComplete);
    conn_opts.cleansession = 1;

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);

            char message[64];
            snprintf(message, sizeof(message), "Temperature: %d, Humidity: %d", temp, hum);

            MQTTClient_message pubmsg = MQTTClient_message_initializer;
            MQTTClient_deliveryToken token;
            pubmsg.payload = message;
            pubmsg.payloadlen = (int)strlen(message);
            pubmsg.qos = 1;
            pubmsg.retained = 0;
            MQTTClient_publishMessage(client, DEFAULT_MQTT_TOPIC, &pubmsg, &token);
            printf("Message published\n");
        } else {
            puts("Failed to read from DHT sensor");
        }

        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME);
    }

    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}