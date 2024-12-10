#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stddef.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "evtimer_msg.h"
#include "net/sock/udp.h"
#include "dht_params.h"
#include "dht.h"
#include "string.h"
#include "gpio.h"
#include "MQTTClient.h"
#include "msg.h"
#include "emcute.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "timex.h"
#include "kernel_defines.h"
#include "thread.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "mutex.h"
#include "stdbool.h"
#include "time_units.h"

#define MQTT_ADDRESS     "tcp://47.102.103.1:1883"
#define CLIENTID         "RIOT_Client"
#define DEFAULT_TOPIC    "sensor/data"
#define QOS              1
#define TIMEOUT          10000L
#define SLEEP_TIME       (60U)

static dht_t dht;
MQTTClient client;

// Callback when the connection is lost
void connlost(void *context, char *cause) {
    printf("Connection lost: %s\n", cause);
}

// Main function to connect and publish
int main(void) {
    int rc;
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Initialize the DHT sensor
    dht.params.pin = GPIO_PIN(0, 0); // Adjust for the correct pin
    if (dht_init(&dht, &dht.params) != DHT_OK) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Create and configure the MQTT client
    MQTTClient_create(&client, MQTT_ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, connlost, NULL, NULL);

    // Configure connection options
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to the broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }
    printf("Connected to MQTT Broker\n");

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            char payload[50];
            snprintf(payload, sizeof(payload), "Temperature: %d, Humidity: %d", temp, hum);
            
            MQTTClient_message pubmsg = MQTTClient_message_initializer;
            pubmsg.payload = payload;
            pubmsg.payloadlen = strlen(payload);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            MQTTClient_deliveryToken token;

            MQTTClient_publishMessage(client, DEFAULT_TOPIC, &pubmsg, &token);
            printf("Waiting for publication of %s on topic %s for client with ClientID: %s\n",
                   payload, DEFAULT_TOPIC, CLIENTID);
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);
        } else {
            puts("Failed to read from DHT sensor");
        }

        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME);
    }

    // Disconnect from the broker
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}